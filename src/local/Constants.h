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

#include <gf/Time.h>
#include <gf/Vector.h>

static constexpr gf::Vector2i ScreenSize = { 1024, 768 };

// Map constants
static constexpr float TileSize = 100.0f; // pixel
static constexpr gf::Vector2u MazeBounds = { 51, 51 }; // Number of tiles
static constexpr gf::Vector2u MazeCenter = { 25, 25 }; // Tiles coordinates

static constexpr gf::Vector2u WorldOffset = { 5, 5 }; // Number of tiles
// static constexpr gf::Vector2u WorldBounds = { (MazeBounds.data[0] * 2) + WorldOffset.data[0], MazeBounds.data[1] + WorldOffset.data[1] }; // Number of tiles
static constexpr gf::Vector2u WorldBounds = { (MazeBounds.x * 2) + WorldOffset.x, MazeBounds.y + WorldOffset.y }; // Number of tiles
static constexpr gf::Vector2u WorldCenter = { WorldBounds.x / 2, WorldBounds.y / 2 }; // Tiles coordinates
static constexpr gf::Vector2f WorldSize = { WorldBounds.x * TileSize, WorldBounds.y * TileSize }; // pixel // dirty

static constexpr gf::Vector2f ViewSize = { 9.75f * TileSize, 9.75f * TileSize }; //  World units

static constexpr gf::Time TimeoutTurn = gf::seconds(3.0f); // Seconds

static constexpr float MonsterSpawnRate = 0.05f;

static constexpr int TotalPlayers = 2;

#endif //_LOCAL_CONSTANTS_H
