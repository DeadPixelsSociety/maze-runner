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

#ifndef _LOCAL_CONSTANTS_H
#define _LOCAL_CONSTANTS_H

#include <gf/Vector.h>

static constexpr gf::Vector2i ScreenSize = { 1024, 768 };

// Map constants
static constexpr float TileSize = 100.0f; // pixel
static constexpr gf::Vector2u WorldBounds = { 21, 21 }; // Number of tiles
static constexpr gf::Vector2u WorldCenter = { 10, 10 }; // Tiles coordinates
// static constexpr gf::Vector2f WorldSize = WorldBounds * TileSize; // pixel // Not compile, why?
static constexpr gf::Vector2f WorldSize = { WorldBounds.data[0] * TileSize, WorldBounds.data[1] * TileSize }; // pixel // dirty
static constexpr float TimeoutTurn = 3.0f; // Seconds

#endif //_LOCAL_CONSTANTS_H
