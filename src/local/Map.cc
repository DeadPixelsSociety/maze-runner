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

#include "Constants.h"
#include "Map.h"
#include "Singletons.h"

Map::Map() :
    gf::Entity(10)
    , m_tilesetTexture(gResourceManager().getTexture("map_tileset.png"))
    , m_layer(WorldBounds) {
    // Init the layer
    m_layer.setTileSize({ 480, 480 });
    m_layer.setBlockSize({ static_cast<unsigned>(TileSize), static_cast<unsigned>(TileSize) });
    m_layer.setTexture(m_tilesetTexture);

    for (unsigned col = 0; col < WorldBounds.width; ++col) {
        for (unsigned row = 0; row < WorldBounds.height; ++row) {
            if (row == WorldCenter.y && col > 0 && col < WorldBounds.width - 1) {
                m_layer.setTile({ col, row }, TileType::Floor);
            }
            else if (row == WorldCenter.y - 1 && col > 0 && col < WorldBounds.width - 1) {
                m_layer.setTile({ col, row }, TileType::Floor);
            }
            else {
                m_layer.setTile({ col, row }, TileType::Wall);
            }
        }
    }

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
    move->isValid = false;

    switch (move->direction) {
    case gf::Direction::Up:
        if (m_layer.getTile(gf::Vector2u(move->position.x, move->position.y - 1)) == TileType::Floor) {
            --move->position.y;
            move->isValid = true;
        }
        break;
    case gf::Direction::Down:
        if (m_layer.getTile(gf::Vector2u(move->position.x, move->position.y + 1)) == TileType::Floor) {
            ++move->position.y;
            move->isValid = true;
        }
        break;
    case gf::Direction::Right:
        if (m_layer.getTile(gf::Vector2u(move->position.x + 1, move->position.y)) == TileType::Floor) {
            ++move->position.x;
            move->isValid = true;
        }
        break;
    case gf::Direction::Left:
        if (m_layer.getTile(gf::Vector2u(move->position.x - 1, move->position.y)) == TileType::Floor) {
            --move->position.x;
            move->isValid = true;
        }
        break;
    default:
        assert(false);
    }

    return gf::MessageStatus::Keep;
}
