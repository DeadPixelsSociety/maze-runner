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

    // Create each rooms
    root.traverseInvertedLevelOrder([this](const gf::SpaceTree& node) {
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

        return true;
    });

    // Create corridor
    createCorridor(&root);
}

void Map::createCorridor(const gf::SpaceTree *root) {
    assert(root != nullptr);

    if (root->isLeaf()) {
        gf::Log::debug("Leaf\n");
    }
    else if (root->getLeftChild()->isLeaf() && !root->getRightChild()->isLeaf()){
        gf::Log::debug("Left Leaf\n");
        return;
    }
    else if (!root->getLeftChild()->isLeaf() && root->getRightChild()->isLeaf()){
        gf::Log::debug("Right Leaf\n");
        return;
    }
    else if (root->getLeftChild()->isLeaf() && root->getRightChild()->isLeaf()) {
        // if (!root->getLeftChild()->isLeaf()) {
        //     createCorridor(root->getLeftChild());
        //     return;
        // }
        // if (!root->getRightChild()->isLeaf()) {
        //     createCorridor(root->getRightChild());
        //     return;
        // }

        assert(root->getLeftChild()->isLeaf() && root->getRightChild()->isLeaf());

        auto centerLeft = root->getLeftChild()->getArea().getCenter();
        auto centerRight = root->getRightChild()->getArea().getCenter();

        // Create horizontal corridor
        if (centerRight.col != centerLeft.col) {
            unsigned minCol = 0;
            unsigned maxCol = 0;
            assert(centerRight.row == centerLeft.row);
            unsigned row = centerRight.row;

            if (centerRight.col > centerLeft.col) {
                minCol = centerLeft.col;
                maxCol = centerRight.col;
            }
            else {
                minCol = centerRight.col;
                maxCol = centerLeft.col;
            }

            for (unsigned col = minCol; col <= maxCol; ++col) {
                m_layer.setTile({ col, row }, TileType::Floor);
            }
        }
        // Create vertical corridor
        else {
            unsigned minRow = 0;
            unsigned maxRow = 0;
            assert(centerRight.col == centerLeft.col);
            unsigned col = centerRight.col;

            if (centerRight.row > centerLeft.row) {
                minRow = centerLeft.row;
                maxRow = centerRight.row;
            }
            else {
                minRow = centerRight.row;
                maxRow = centerLeft.row;
            }

            for (unsigned row = minRow; row <= maxRow; ++row) {
                m_layer.setTile({ col, row }, TileType::Floor);
            }
        }

        // if (centerRight.col > centerLeft.col) {
        //     colStart = centerLeft.col;
        //     colEnd = centerRight.col;
        // }
        // else {
        //     colStart = centerRight.col;
        //     colEnd = centerLeft.col;
        // }
        //
        // if (centerRight.row > centerLeft.row) {
        //     rowStart = centerLeft.row;
        //     rowEnd = centerRight.row;
        // }
        // else {
        //     rowStart = centerRight.row;
        //     rowEnd = centerLeft.row;
        // }
        //
        // gf::Log::debug("col %u -> %u\n", colStart, colEnd);
        // gf::Log::debug("row %u -> %u\n", rowStart, rowEnd);
        //
        // for (unsigned col = colStart; col <= colEnd; ++col) {
        //     for (unsigned row = rowStart; row <= rowEnd; ++row) {
        //         gf::Log::debug("dig\n");
        //         m_layer.setTile({ col, row }, TileType::Floor);
        //     }
        // }
        // gf::SpaceTree *father = root->getFather();
        // assert(father != nullptr);
        //
        // // Create the corridor
        // auto centerLeft = node.getArea().getCenter();
        // auto centerLeft = node.getArea().getCenter();
        // for (int col = 0; )
        return;
    }

    createCorridor(root->getLeftChild());
    createCorridor(root->getRightChild());
}
