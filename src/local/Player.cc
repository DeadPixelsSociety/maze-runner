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

#include "Constants.h"
#include "Player.h"
#include "Singletons.h"

uint8_t Player::s_totalPlayers = 0;

static constexpr float SpriteXScale = TileSize / 121.0f;
static constexpr float SpriteYScale = TileSize / 153.0f;

Player::Player(const gf::Vector2i position) :
    gf::Entity(20)
    , m_position(position)
    , m_wantsMove(false)
    , m_direction(gf::Direction::Left)
    , m_sightDirection(gf::Direction::Left)
    , m_isHisTurn(false)
    , m_timeElapsed(0.0f)
    , m_numPlayer(s_totalPlayers + 1)
    , m_playerTexture(gResourceManager().getTexture("player" + std::to_string(m_numPlayer) + ".png")) {
    // Register message handler
    gMessageManager().registerHandler<EndTurnMessage>(&Player::onEndTurn, this);

    // Inc the total of players
    ++s_totalPlayers;

    gf::Log::debug("Player #%d created\n", m_numPlayer);
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
        move.position = m_position;
        move.direction = m_direction;
        move.isValid = false;

        gMessageManager().sendMessage(&move);

        // Update position
        m_position = move.position;
        m_wantsMove = false;

        // End of turn
        if (move.isValid) {
            setEndTurn();
        }
    }
    else if (m_isHisTurn) {
        m_timeElapsed += time.asSeconds();

        if (m_timeElapsed >= TimeoutTurn) {
            // End of turn
            setEndTurn();
        }
    }
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
    if (endTurn->player != this) {
        m_timeElapsed = 0.0;
        m_isHisTurn = true;
    }

    return gf::MessageStatus::Keep;
}

void Player::setEndTurn() {
    m_wantsMove = false;
    m_isHisTurn = false;
    m_timeElapsed = 0.0;
    EndTurnMessage msg;
    msg.player = this;
    gMessageManager().sendMessage(&msg);
}
