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

#ifndef _LOCAL_MONSTER_MANAGER_H
#define _LOCAL_MONSTER_MANAGER_H

#include <array>
#include <vector>

#include <gf/Entity.h>
#include <gf/Texture.h>

#include "Constants.h"
#include "Messages.h"

class MonsterManager : public gf::Entity {
public:
    MonsterManager();

    virtual void update(gf::Time time) override;
    virtual void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

    gf::MessageStatus onMonsterSpawn(gf::Id id, gf::Message *msg);
    gf::MessageStatus onPlayersLocation(gf::Id id, gf::Message *msg);

private:
    void addNewMonster(gf::Vector2i position);

private:
    static constexpr int TotalMonsterType = 1;
    enum MonsterType: uint8_t {
        Demon,
    };

    struct Monster {
        MonsterType type;
        gf::Vector2i position; // Tile position
        gf::Direction sightDirection;
    };

private:
    std::vector<Monster> m_monsters;
    std::array<gf::Vector2i, TotalPlayers> m_playerPositions;
    gf::Texture &m_demonTexture;
};

#endif // _LOCAL_MONSTER_MANAGER_H
