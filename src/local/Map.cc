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

static constexpr int RoomMinSize = 5;
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
    gf::SpaceTree root(gf::RectI({ 1, 1 }, WorldBounds - 2));
    root.splitRecursive(gRandom(), 8, { RoomMinSize, RoomMinSize }, { RoomMaxSize, RoomMaxSize });

    // Create each rooms
    root.traverseInvertedLevelOrder([this](const gf::SpaceTree& node) {
        if (!node.isLeaf()) {
            return true;
        }

        auto area = node.getArea();

        // Random sub-rect (x, y) coordinates and (width, height) size
        assert(RoomMinSize <= area.width);
        int width = gRandom().computeUniformInteger(RoomMinSize - 2, area.width - 2);
        assert(RoomMinSize <= area.height);
        int height = gRandom().computeUniformInteger(RoomMinSize - 2, area.height - 2);
        assert(1 <= area.width - width - 1);
        int x = area.left + gRandom().computeUniformInteger(1, area.width - width - 1);
        assert(1 <= area.height - height - 1);
        int y = area.top + gRandom().computeUniformInteger(1, area.height - height - 1);

        // Create the room
        for (unsigned col = 0; col < static_cast<unsigned>(width); ++col) {
            for (unsigned row = 0; row < static_cast<unsigned>(height); ++row) {
                m_layer.setTile({ x + col, y + row }, TileType::Floor);
            }
        }

        return true;
    });


}
