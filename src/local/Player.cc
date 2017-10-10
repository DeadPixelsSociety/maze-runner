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

#include <cassert>

#include <gf/Color.h>
#include <gf/Shapes.h>
#include <gf/RenderTarget.h>

#include "Constants.h"
#include "Player.h"

Player::Player(const gf::Vector2i position) :
    gf::Entity(20)
    , m_position(position)
    , m_wantsMove(false)
    , m_direction(gf::Direction::Left)
    , m_timeElapsed(0.0f) {
    // Constructor
}

void Player::goTo(const gf::Direction direction) {
    m_wantsMove = true;

    switch (direction) {
    case gf::Direction::Up:
        m_direction = gf::Direction::Up;
        break;
    case gf::Direction::Down:
        m_direction = gf::Direction::Down;
        break;
    case gf::Direction::Right:
        m_direction = gf::Direction::Right;
        break;
    case gf::Direction::Left:
        m_direction = gf::Direction::Left;
        break;
    default:
        assert(false);
    }
}

void Player::update(gf::Time time) {
    // Temporary
    if (m_wantsMove == true) {
        switch (m_direction) {
        case gf::Direction::Up:
            m_position.y--;
            break;
        case gf::Direction::Down:
            m_position.y++;
            break;
        case gf::Direction::Right:
            m_position.x++;
            break;
        case gf::Direction::Left:
            m_position.x--;
            break;
        default:
            assert(false);
        }

        m_wantsMove = false;
    }
}

void Player::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::CircleShape circle(50);
    circle.setColor(gf::Color::Red);
    circle.setPosition(m_position * TileSize);

    target.draw(circle, states);
}
