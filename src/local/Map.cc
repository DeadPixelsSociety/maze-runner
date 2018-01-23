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
    auto offset = WorldOffset / 2;
    gf::SpaceTree root(gf::RectI(offset, MazeBounds));
    root.splitRecursive(gRandom(), 8, { RoomMinSize, RoomMinSize }, { RoomMaxSize, RoomMaxSize });

    std::vector<gf::Vector2i> roomCoordinates;

    // Create each rooms
    root.traverseInvertedLevelOrder([this, &roomCoordinates, offset](const gf::SpaceTree& node) {
        if (!node.isLeaf()) {
            return true;
        }

        auto area = node.getArea();
        // Create the room
        for (unsigned col = offset.x-1; col < static_cast<unsigned>(area.width-offset.x); ++col) {
            for (unsigned row = offset.x-1; row < static_cast<unsigned>(area.height-offset.x); ++row) {
                m_layer.setTile({ area.left + col, area.top + row + 1 }, TileType::Floor);
                m_layer.setTile(gf::Vector2u(WorldBounds.width - (area.left + col) - 1, area.top + row + 1), TileType::Floor);
            }
        }

        roomCoordinates.push_back(node.getArea().getCenter());

        return true;
    });

    // Create corridor
    createCorridor(roomCoordinates);

    // Create exit
    createExit(roomCoordinates);

    // Create entry
    createSpawn(roomCoordinates);
}

void Map::createCorridor(std::vector<gf::Vector2i> roomCoordinates) {
    // Init the structure
    gf::Vector2i room1;
    gf::Vector2i room2;
    std::vector<gf::Vector2i> alreadyConnectedRooms;

    // Pick the first connected room
    int index = gRandom().computeUniformInteger<unsigned>(0, roomCoordinates.size() - 1);
    alreadyConnectedRooms.push_back(roomCoordinates[index]);
    roomCoordinates.erase(roomCoordinates.begin() + index);

    do {
        // Random pick two rooms
        unsigned indexRoom1 = gRandom().computeUniformInteger<unsigned>(0, roomCoordinates.size() - 1);
        unsigned indexRoom2 = gRandom().computeUniformInteger<unsigned>(0, alreadyConnectedRooms.size() - 1);
        room1 = roomCoordinates[indexRoom1];
        room2 = alreadyConnectedRooms[indexRoom2];

        // Update the vector
        roomCoordinates.erase(roomCoordinates.begin() + indexRoom1);
        alreadyConnectedRooms.push_back(room1);

        digCorridor(room1, room2);
    } while (roomCoordinates.size() > 0);
}

void Map::createExit(std::vector<gf::Vector2i> roomCoordinates) {
    // Choose the y coordinate
    unsigned y = gRandom().computeUniformInteger<unsigned>(2, MazeBounds.y - 2);

    for (unsigned col = WorldCenter.x - 1; col <= WorldCenter.x + 1; ++col) {
        for (unsigned row = y; row <= y + 2; ++row) {
            m_layer.setTile(gf::Vector2u(col, row), TileType::Floor);
        }
    }

    // Pick only the most right room
    std::vector<gf::Vector2i> leftRooms;
    for (auto coord: roomCoordinates) {
        if (static_cast<unsigned>(coord.x) >= MazeBounds.y - 5) {
            leftRooms.push_back(coord);
        }
    }

    // Choose one room
    unsigned indexExitRoom = gRandom().computeUniformInteger<unsigned>(0, leftRooms.size() - 1);

    // Connect the exit
    digCorridor({ static_cast<int>(WorldCenter.x), static_cast<int>(y) + 1 }, leftRooms[indexExitRoom] - 1);

    // Connect the two map part
    for (int i = 0; i < 5; ++i) {
        // Choose one room
        unsigned indexRoom = gRandom().computeUniformInteger<unsigned>(0, leftRooms.size() - 1);

        digCorridor(leftRooms[indexRoom], { static_cast<int>(WorldBounds.width) - leftRooms[indexRoom].x, leftRooms[indexRoom].y });
    }
}

void Map::createSpawn(std::vector<gf::Vector2i> roomCoordinates) {
    // Choose a room
    unsigned indexRoom = gRandom().computeUniformInteger<unsigned>(0, roomCoordinates.size() - 1);

    while(roomCoordinates[indexRoom].x >= MazeBounds.width * 0.25f) {
        indexRoom = gRandom().computeUniformInteger<unsigned>(0, roomCoordinates.size() - 1);
    }

    // Set the initial position for player 1
    SpawnLocationMessage spawn;
    spawn.position = { roomCoordinates[indexRoom].x, roomCoordinates[indexRoom].y };
    spawn.numPlayer = 1;
    gMessageManager().sendMessage(&spawn);

    // Set the initial position for player 2
    spawn.position = { static_cast<int>(WorldBounds.width) - roomCoordinates[indexRoom].x - 1, roomCoordinates[indexRoom].y };
    spawn.numPlayer = 2;
    gMessageManager().sendMessage(&spawn);
}

void Map::digCorridor(const gf::Vector2i &room1, const gf::Vector2i &room2, TileType tileType) {
    // Define the step
    int xStep = 0;
    int yStep = 0;

    // Define direction
    if (room1.x < room2.x) {
        xStep = 1;
    }
    else {
        xStep = -1;
    }
    if (room1.y < room2.y) {
        yStep = 1;
    }
    else {
        yStep = -1;
    }

    // Create the corridor
    if (gRandom().computeBernoulli(0.5)) { // Horizontal first
        for (int x = room1.x; x != room2.x; x += xStep) {
            m_layer.setTile(gf::Vector2u(x, room1.y + 1), tileType);
            m_layer.setTile(gf::Vector2u(WorldBounds.width - x - 1, room1.y + 1), tileType);
        }
        for (int y = room1.y; y != room2.y; y += yStep) {
            m_layer.setTile(gf::Vector2u(room2.x, y + 1), tileType);
            m_layer.setTile(gf::Vector2u(WorldBounds.width - room2.x - 1, y + 1), tileType);
        }
    }
    else {
        for (int y = room1.y; y != room2.y; y += yStep) { // Vertical first
            m_layer.setTile(gf::Vector2u(room1.x, y + 1), tileType);
            m_layer.setTile(gf::Vector2u(WorldBounds.width - room1.x - 1, y + 1), tileType);
        }
        for (int x = room1.x; x != room2.x; x += xStep) {
            m_layer.setTile(gf::Vector2u(x, room2.y + 1), tileType);
            m_layer.setTile(gf::Vector2u(WorldBounds.width - x - 1, room2.y + 1), tileType);
        }
    }
}
