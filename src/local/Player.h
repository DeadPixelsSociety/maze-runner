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

#ifndef _PLAYER_LOCAL_H
#define _PLAYER_LOCAL_H

#include <gf/Direction.h>
#include <gf/Entity.h>
#include <gf/Vector.h>

#include "Messages.h"

class Player: public gf::Entity {
public:
    Player(const gf::Vector2i position, const gf::Direction sight = gf::Direction::Left);

    void goTo(const gf::Direction direction);

    virtual void update(gf::Time time) override;
    virtual void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

    gf::MessageStatus onEndTurn(gf::Id id, gf::Message *msg);
    gf::MessageStatus onMovePlayer(gf::Id id, gf::Message *msg);

    bool hisTurn();
    void setEndTurn();

private:
    gf::Vector2i m_position;
    bool m_wantsMove;
    gf::Direction m_direction;
    gf::Direction m_sightDirection;
    bool m_isHisTurn;
    gf::Time m_turnTime;

    static uint8_t s_totalPlayers;
    uint8_t m_numPlayer;

    gf::Texture &m_playerTexture;
};

#endif // _PLAYER_LOCAL_H
