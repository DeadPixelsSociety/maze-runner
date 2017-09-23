/*
 * Run and Crawl - A student indie game
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

#include <gf/Color.h>
#include <gf/Shapes.h>
#include <gf/RenderTarget.h>

#include "Constants.h"
#include "Player.h"

Player::Player(const gf::Vector2i position) :
    m_position(position) {
    // Constructor
}

void Player::update(gf::Time time) {
    // Nothing
}

void Player::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::CircleShape circle(50);
    circle.setColor(gf::Color::Red);
    circle.setAnchor(gf::Anchor::Center);
    circle.setPosition(m_position * TileSize - 0.5f * TileSize);

    target.draw(circle, states);
}
