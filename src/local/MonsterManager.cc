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
#include <gf/Shapes.h>

#include "Singletons.h"

MonsterManager::MonsterManager()
: gf::Entity(50) {
    // Register message handler
    gMessageManager().registerHandler<MonsterSpawnMessage>(&MonsterManager::onMonsterSpawn, this);
}

void MonsterManager::update(gf::Time time) {
    // Do something
}

void MonsterManager::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    for (auto &monster: m_monsters) {
        gf::CircleShape circle(TileSize * 0.5f);
        circle.setColor(gf::Color::Black);
        circle.setPosition(monster.position * TileSize);

        circle.draw(target, states);
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

void MonsterManager::addNewMonster(gf::Vector2i position) {
    uint8_t randomType = gRandom().computeUniformInteger(0, TotalMonsterType - 1);

    Monster monster;
    monster.type = static_cast<MonsterType>(randomType);

    uint8_t randomDirection = gRandom().computeUniformInteger(0, 3);
    monster.sightDirection = static_cast<gf::Direction>(randomDirection);

    monster.position = position;

    m_monsters.push_back(monster);
}
