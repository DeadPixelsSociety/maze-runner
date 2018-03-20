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

#include "MonsterManager.h"

#include <cassert>

#include <gf/Log.h>
#include <gf/Sprite.h>
#include <gf/VectorOps.h>

#include "Map.h"
#include "Singletons.h"

MonsterManager::MonsterManager()
: gf::Entity(50)
, m_demonTexture(gResourceManager().getTexture("demon.png")) {
    // Register message handler
    gMessageManager().registerHandler<MonsterSpawnMessage>(&MonsterManager::onMonsterSpawn, this);
    gMessageManager().registerHandler<EndTurnMessage>(&MonsterManager::onEndTurn, this);
    gMessageManager().registerHandler<MovePlayerMessage>(&MonsterManager::onMovePlayer, this);
}

void MonsterManager::update(gf::Time time) {
    if (m_activeTurn) {
        // We check the location for all monsters
        for (auto &monster: m_monsters) {
            MonsterLocationMessage locMessage;
            locMessage.position = monster.position;
            gMessageManager().sendMessage(&locMessage);

            monster.position = locMessage.position;
        }

        // Set the new turn
        m_activeTurn = false;
        EndTurnMessage msg;
        msg.playerID = 1;
        gMessageManager().sendMessage(&msg);
    }
}

void MonsterManager::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    static constexpr float SpriteSize = 480.0f;

    for (auto &monster: m_monsters) {
        gf::Sprite sprite;

        switch (monster.type) {
        case MonsterType::Demon:
            sprite.setTexture(m_demonTexture);
            break;

        default:
            assert(false);
        }

        sprite.scale(TileSize / SpriteSize);
        sprite.setPosition(TileSize * monster.position);

        sprite.draw(target, states);
    }
}

gf::MessageStatus MonsterManager::onMonsterSpawn(gf::Id id, gf::Message *msg) {
    assert(id == MonsterSpawnMessage::type);
    MonsterSpawnMessage *spawn = reinterpret_cast<MonsterSpawnMessage*>(msg);

    addNewMonster(spawn->position);

    return gf::MessageStatus::Keep;
}

gf::MessageStatus MonsterManager::onPlayersLocation(gf::Id id, gf::Message *msg) {
    assert(id == PlayersLocationMessage::type);
    PlayersLocationMessage *move = reinterpret_cast<PlayersLocationMessage*>(msg);

    m_playerPositions[move->numPlayer - 1] = move->position;

    return gf::MessageStatus::Keep;
}

gf::MessageStatus MonsterManager::onEndTurn(gf::Id id, gf::Message *msg) {
    assert(id == EndTurnMessage::type);
    EndTurnMessage *endTurn = reinterpret_cast<EndTurnMessage*>(msg);

    // If it's the turn of monster
    if (endTurn->playerID == 0) {
        m_activeTurn = true;
    }

    return gf::MessageStatus::Keep;
}

gf::MessageStatus MonsterManager::onMovePlayer(gf::Id id, gf::Message *msg) {
    assert(id == MovePlayerMessage::type);
    MovePlayerMessage *move = reinterpret_cast<MovePlayerMessage*>(msg);

    // If the movement is already invalid, we don't check anything
    if (!move->isValid) {
        return gf::MessageStatus::Keep;
    }

    gf::Vector2u newPostion = Map::computeNextPosition(move->position, move->direction);

    // If a monster is present
    for (auto &monster: m_monsters) {
        if (newPostion == static_cast<gf::Vector2u>(monster.position)) {
            move->isValid = false;
            break;
        }
    }

    return gf::MessageStatus::Keep;
}

void MonsterManager::addNewMonster(gf::Vector2i position) {
    uint8_t randomType = gRandom().computeUniformInteger(0, TotalMonsterType - 1);

    Monster monster;
    monster.type = static_cast<MonsterType>(randomType);

    uint8_t randomDirection = gRandom().computeUniformInteger(0, 3);
    monster.sightDirection = static_cast<gf::Direction>(randomDirection);

    monster.position = position;

    m_monsters.push_back(monster);
}
