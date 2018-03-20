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

#ifndef _MAP_LOCAL_H
#define _MAP_LOCAL_H

#include <gf/Entity.h>
#include <gf/Map.h>
#include <gf/SpaceTree.h>
#include <gf/TileLayer.h>
#include <gf/VectorOps.h>

#include "Constants.h"
#include "Messages.h"

class Map: public gf::Entity {
public:
    Map();

    virtual void update(gf::Time time) override;
    virtual void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

    gf::MessageStatus onMovePlayer(gf::Id id, gf::Message *msg);

private:
    enum TileType : uint8_t {
        Floor = 0,
        Wall = 1,
        DebugRed = 2,
        DebugDark = 3,
        DebugYellow = 4,
        DebugBlue = 5,
        DebugWhite = 6,
    };

private:
    void generate();
    void createCorridor(std::vector<gf::Vector2i> roomCoordinates);
    void createExit(std::vector<gf::Vector2i> roomCoordinates);
    void createSpawn(std::vector<gf::Vector2i> roomCoordinates);
    void digCorridor(const gf::Vector2i &room1, const gf::Vector2i &room2);
    bool moveIsValid(TileType tileType) const;
    void setFloor(gf::Vector2u position, bool drawSymetric = true);

private:
    gf::Texture &m_tilesetTexture;
    gf::TileLayer m_layer;
    int m_level;
    gf::Vector2i m_exitCoordinates;
    std::array<gf::Vector2i, TotalPlayers> m_playerPositions;
    gf::SquareMap m_squareMap;
};

#endif // _MAP_LOCAL_H
