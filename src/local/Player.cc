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
#include <gf/Log.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>
#include <gf/VectorOps.h>

#include "Constants.h"
#include "Player.h"
#include "Singletons.h"

uint8_t Player::s_totalPlayers = 0;

static constexpr float SpriteXScale = TileSize / 121.0f;
static constexpr float SpriteYScale = TileSize / 153.0f;

Player::Player(const gf::Direction sight) :
    gf::Entity(20)
    , m_position({ 0, 0 })
    , m_wantsMove(false)
    , m_direction(gf::Direction::Left)
    , m_sightDirection(sight)
    , m_isHisTurn(false)
    , m_numPlayer(s_totalPlayers + 1)
    , m_playerTexture(gResourceManager().getTexture("player" + std::to_string(m_numPlayer) + ".png")) {
    // Register message handler
    gMessageManager().registerHandler<EndTurnMessage>(&Player::onEndTurn, this);
    gMessageManager().registerHandler<MovePlayerMessage>(&Player::onMovePlayer, this);
    gMessageManager().registerHandler<SpawnLocationMessage>(&Player::onSpawnLocation, this);

    // Inc the total of players
    ++s_totalPlayers;
}

void Player::goTo(const gf::Direction direction) {
    // If is not his trun, we skip
    if (!m_isHisTurn) {
        return;
    }

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
        m_sightDirection = gf::Direction::Right;
        break;
    case gf::Direction::Left:
        m_direction = gf::Direction::Left;
        m_sightDirection = gf::Direction::Left;
        break;
    default:
        assert(false);
    }
}

void Player::update(gf::Time time) {
    // If the player has his turn and he want move
    if (m_wantsMove && m_isHisTurn) {
        // Send move
        MovePlayerMessage move;
        move.newPosition = computeNextPosition(m_position, m_direction);
        move.isValid = true;
        move.numPlayer = m_numPlayer;

        gMessageManager().sendMessage(&move);

        // Update position
        m_wantsMove = false;
        if (move.isValid) {
            m_position = move.newPosition;

            // End of turn
            setEndTurn();
        }
    }
    else if (m_isHisTurn) {
        m_turnTime.addTo(time);

        if (m_turnTime >= TimeoutTurn) {
            // End of turn
            setEndTurn();
        }
    }

    // Send the current position
    PlayersLocationMessage location;
    location.position = m_position;
    location.numPlayer = m_numPlayer;

    gMessageManager().sendMessage(&location);
}

void Player::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Sprite sprite;
    sprite.setTexture(m_playerTexture);

    gf::Vector2f coord(TileSize * m_position.x + TileSize * 0.5f, TileSize * m_position.y + TileSize * 0.5f);
    sprite.setPosition(coord);
    sprite.setAnchor(gf::Anchor::Center);

    if (m_sightDirection == gf::Direction::Right) {
        sprite.setScale({-SpriteXScale, SpriteYScale});
    }
    else {
        sprite.setScale({SpriteXScale, SpriteYScale});
    }

    target.draw(sprite, states);
}

gf::MessageStatus Player::onEndTurn(gf::Id id, gf::Message *msg) {
    assert(id == EndTurnMessage::type);
    EndTurnMessage *endTurn = reinterpret_cast<EndTurnMessage*>(msg);

    // If is not the current player
    if (endTurn->playerID == m_numPlayer) {
        m_turnTime = gf::Time::zero();
        m_isHisTurn = true;
    }

    return gf::MessageStatus::Keep;
}

gf::MessageStatus Player::onMovePlayer(gf::Id id, gf::Message *msg) {
    assert(id == MovePlayerMessage::type);
    MovePlayerMessage *move = reinterpret_cast<MovePlayerMessage*>(msg);

    // If the movement is already invalid, we don't check anything
    if (!move->isValid) {
        return gf::MessageStatus::Keep;
    }

    move->isValid = move->newPosition != m_position;

    return gf::MessageStatus::Keep;
}

gf::MessageStatus Player::onSpawnLocation(gf::Id id, gf::Message *msg) {
    assert(id == SpawnLocationMessage::type);
    SpawnLocationMessage *spawnLocation = reinterpret_cast<SpawnLocationMessage*>(msg);

    // If is not the current player
    if (spawnLocation->numPlayer != m_numPlayer) {
        return gf::MessageStatus::Keep;
    }

    m_position = spawnLocation->position;

    return gf::MessageStatus::Keep;
}

void Player::setEndTurn() {
    m_wantsMove = false;
    m_isHisTurn = false;
    m_turnTime = gf::Time::zero();
    EndTurnMessage msg;
    msg.playerID = (m_numPlayer + 1) % (Player::s_totalPlayers + 1);
    gMessageManager().sendMessage(&msg);
}

gf::Vector2u Player::computeNextPosition(gf::Vector2u curPosition, gf::Direction direction) {
    gf::Vector2u newPostion = curPosition;

    switch (direction) {
    case gf::Direction::Up:
        newPostion.y--;
        break;
    case gf::Direction::Down:
        newPostion.y++;
        break;
    case gf::Direction::Right:
        newPostion.x++;
        break;
    case gf::Direction::Left:
        newPostion.x--;
        break;
    default:
        assert(false);
    }

    return newPostion;
}
