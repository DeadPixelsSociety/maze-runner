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

Map::Map() :
    gf::Entity(10) {
    // Nothing
}

void Map::update(gf::Time time) {
    // Nothing
}

void Map::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::RectangleShape background;
    background.setSize(WorldSize);
    background.setPosition({ 0.0f, 0.0f });
    background.setColor(gf::Color::Black);

    target.draw(background, states);
}
