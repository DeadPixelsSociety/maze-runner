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

#include "ViewManager.h"

#include <cassert>

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Text.h>

ViewManager::ViewManager(gf::EntityContainer &mainEntities, gf::EntityContainer &hudEntities, gf::ExtendView &player1View, gf::ExtendView &player2View, gf::ScreenView &hudView)
: m_currentState(State::InGame)
, m_font(gResourceManager().getFont("russo_one.ttf"))
, m_mainEntities(mainEntities)
, m_hudEntities(hudEntities)
, m_player1View(player1View)
, m_player2View(player2View)
, m_hudView(hudView)
, m_numWinner(0) {
    gMessageManager().registerHandler<GameOverMessage>(&ViewManager::onGameOver, this);
}

void ViewManager::update(gf::Time time) {
    switch (m_currentState) {
    case State::InGame:
        m_mainEntities.update(time);
        m_hudEntities.update(time);
        break;

    case State::GameOver:
        // Nothing
        break;

    default:
        assert(false);
    }
}

void ViewManager::render(gf::RenderTarget &target) {
    render(target, gf::RenderStates());
}

void ViewManager::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    switch (m_currentState) {
    case State::InGame:
        // Player1 view
        target.setView(m_player1View);
        m_mainEntities.render(target, states);

        // Player2 view
        target.setView(m_player2View);
        m_mainEntities.render(target, states);

        // HUD view
        target.setView(m_hudView);
        m_hudEntities.render(target, states);
        break;

    case State::GameOver:
        target.setView(m_hudView);
        renderGameOverScreen(target, states);
        break;

    default:
        assert(false);
    }
}

gf::MessageStatus ViewManager::onGameOver(gf::Id id, gf::Message *msg) {
    assert(id == GameOverMessage::type);
    auto gameOver = static_cast<GameOverMessage*>(msg);

    m_currentState = GameOver;
    m_numWinner = gameOver->numPlayer;

    return gf::MessageStatus::Keep;
}

void ViewManager::renderGameOverScreen(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coordinates(target);

    gf::Text text;
    text.setString("The player #" + std::to_string(m_numWinner) + " wins!");
    text.setFont(m_font);
    text.setColor(gf::Color::Black);
    text.setParagraphWidth(coordinates.getRelativeSize({ 0.8f, 1.0f }).width); // Not works ?
    text.setCharacterSize(coordinates.getRelativeCharacterSize(0.1f));
    text.setAnchor(gf::Anchor::Center);
    text.setPosition(coordinates.getCenter());

    text.draw(target, states);
}
