/*
 * Maze Runner - A student indie game
 * Copyright (C) 2017-2018 to authors see AUTHORS file
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/SpaceTree.h>

#include "Constants.h"
#include "Map.h"
#include "Singletons.h"

static constexpr int RoomMinSize = 6;
static constexpr int RoomMaxSize = 9;

Map::Map() :
    gf::Entity(10)
    , m_tilesetTexture(gResourceManager().getTexture("map_tileset.png"))
    , m_layer(WorldBounds) {
    // Init the layer
    m_layer.setTileSize({ 480, 480 });
    m_layer.setBlockSize({ static_cast<unsigned>(TileSize), static_cast<unsigned>(TileSize) });
    m_layer.setTexture(m_tilesetTexture);

    generate();

    // Register message handler
    gMessageManager().registerHandler<MovePlayerMessage>(&Map::onMovePlayer, this);
}

void Map::update(gf::Time time) {
    // gf::unused(time);
    // Nothing
}

void Map::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    target.draw(m_layer, states);
}

gf::MessageStatus Map::onMovePlayer(gf::Id id, gf::Message *msg) {
    assert(id == MovePlayerMessage::type);
    MovePlayerMessage *move = reinterpret_cast<MovePlayerMessage*>(msg);

    // If the movement is already invalid, we don't check anything
    if (!move->isValid) {
        return gf::MessageStatus::Keep;
    }

    switch (move->direction) {
    case gf::Direction::Up:
        move->isValid = m_layer.getTile(gf::Vector2u(move->position.x, move->position.y - 1)) == TileType::Floor;
        break;
    case gf::Direction::Down:
        move->isValid = m_layer.getTile(gf::Vector2u(move->position.x, move->position.y + 1)) == TileType::Floor;
        break;
    case gf::Direction::Right:
        move->isValid = m_layer.getTile(gf::Vector2u(move->position.x + 1, move->position.y)) == TileType::Floor;
        break;
    case gf::Direction::Left:
        move->isValid = m_layer.getTile(gf::Vector2u(move->position.x - 1, move->position.y)) == TileType::Floor;
        break;
    default:
        assert(false);
    }

    return gf::MessageStatus::Keep;
}

void Map::generate() {
    // Fill the map with wall
    for (unsigned col = 0; col < WorldBounds.width; ++col) {
        for (unsigned row = 0; row < WorldBounds.height; ++row) {
            m_layer.setTile({ col, row }, TileType::Wall);
        }
    }

    // Split the map into sub rect
    gf::SpaceTree root(gf::RectI({ 0, 0 }, WorldBounds));
    root.splitRecursive(gRandom(), 8, { RoomMinSize, RoomMinSize }, { RoomMaxSize, RoomMaxSize });

    std::vector<gf::Vector2i> roomCoordinates;

    // Create each rooms
    root.traverseInvertedLevelOrder([this, &roomCoordinates](const gf::SpaceTree& node) {
        if (!node.isLeaf()) {
            return true;
        }

        auto area = node.getArea();

        // Create the room
        for (unsigned col = 2; col < static_cast<unsigned>(area.width-2); ++col) {
            for (unsigned row = 2; row < static_cast<unsigned>(area.height-2); ++row) {
                m_layer.setTile({ area.left + col, area.top + row }, TileType::Floor);
            }
        }

        roomCoordinates.push_back(node.getArea().getCenter());

        return true;
    });

    // Create corridor
    createCorridor(roomCoordinates);

    // for (auto &room: roomCoordinates) {
    //     gf::Log::debug("%d ; %d = %d\n", room.x, room.y, gf::euclideanLength(room));
    // }
}

void Map::createCorridor(std::vector<gf::Vector2i> &roomCoordinates) {
    // Sort the rooms by position
    // std::sort(roomCoordinates.begin(), roomCoordinates.end(), [](const gf::Vector2i &room1, const gf::Vector2i &room2){
    //     auto result = gf::lessThan(room1, room2);
    //
    //     return result[0] || result[1];
    //     // return lessThangf::euclideanLength(room1) < gf::euclideanLength(room2);
    //     // return gf::euclideanLength(room1) < gf::euclideanLength(room2);
    //
    //     // return room1.x <= room2.x && room1.y <= room2.y;
    // });

    // Search the first room
    unsigned indexTopLeftRoom = 0;
    gf::Vector2i room1;
    gf::Vector2i room2;

    int colorChoice = 0;

    do {
        indexTopLeftRoom = 0;
        for (unsigned i = 1; i < roomCoordinates.size(); ++i) {
            if (gf::euclideanLength(roomCoordinates[i]) < gf::euclideanLength(roomCoordinates[indexTopLeftRoom])) {
                indexTopLeftRoom = i;
            }
        }

        // Store the first room
        room1 = roomCoordinates[indexTopLeftRoom];
        roomCoordinates.erase(roomCoordinates.begin() + indexTopLeftRoom);

        // Search the second room
        indexTopLeftRoom = 0;
        for (unsigned i = 1; i < roomCoordinates.size(); ++i) {
            if (roomCoordinates[i].x != room1.x && roomCoordinates[i].y != room1.y) {
                continue;
            }

            if (gf::manhattanDistance(room1, roomCoordinates[i]) < gf::manhattanDistance(room1, roomCoordinates[indexTopLeftRoom])) {
                indexTopLeftRoom = i;
            }
        }

        // Store the second room
        room2 = roomCoordinates[indexTopLeftRoom];
        roomCoordinates.erase(roomCoordinates.begin() + indexTopLeftRoom);

        gf::Log::debug("%d ; %d = %d\n", room1.x, room1.y, gf::euclideanDistance(room1, room2));
        gf::Log::debug("%d ; %d = %d\n\n", room2.x, room2.y, gf::euclideanDistance(room2, room1));

        int xStart = -1;
        int xEnd = -1;
        if (room1.x < room2.x) {
            xStart = room1.x;
            xEnd = room2.x;
        }
        else {
            xStart = room2.x;
            xEnd = room1.x;
        }

        int yStart = -1;
        int yEnd = -1;
        if (room1.y < room2.y) {
            yStart = room1.y;
            yEnd = room2.y;
        }
        else {
            yStart = room2.y;
            yEnd = room1.y;
        }

        for (int x = xStart; x <= xEnd; ++x) {
            m_layer.setTile({ x, yStart }, TileType::DebugRed + colorChoice);
        }

        for (int y = yStart; y <= yEnd; ++y) {
            m_layer.setTile({ xEnd, y }, TileType::DebugRed + colorChoice);
        }

        colorChoice = (colorChoice + 1) % 5;
    } while (roomCoordinates.size() > 1);
}
