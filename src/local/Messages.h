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

#ifndef _LOCAL_MESSAGES_H
#define _LOCAL_MESSAGES_H

#include <gf/Direction.h>
#include <gf/Message.h>

class Player;

using namespace gf::literals;

struct EndTurnMessage : public gf::Message {
    static const gf::Id type = "EndTurnMessage"_id; // compile-time definition
    unsigned playerID; // ID of next player
};

struct MovePlayerMessage : public gf::Message {
    static const gf::Id type = "MovePlayerMessage"_id;
    gf::Vector2i position;
    gf::Direction direction;
    uint8_t numPlayer;
    bool isValid;
};

struct SpawnLocationMessage : public gf::Message {
    static const gf::Id type = "SpawnLocationMessage"_id;
    gf::Vector2i position;
    uint8_t numPlayer;
};

struct PlayersLocationMessage : public gf::Message {
    static const gf::Id type = "PlayersLocationMessage"_id;
    gf::Vector2i position; // Tile position
    uint8_t numPlayer;
};

struct GameOverMessage : public gf::Message {
    static const gf::Id type = "GameOverMessage"_id;
    uint8_t numPlayer;
};

struct MonsterSpawnMessage : public gf::Message {
    static const gf::Id type = "MonsterSpawnMessage"_id;
    gf::Vector2i position; // Tile position
};

#endif // _LOCAL_MESSAGES_H
