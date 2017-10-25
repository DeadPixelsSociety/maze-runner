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
#include "HeadUpDisplay.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Text.h>

#include "Singletons.h"

HeadUpDisplay::HeadUpDisplay() :
    gf::Entity(30)
    , m_font(gResourceManager().getFont("font.ttf"))
    , m_currentPlayer(0) {
    // Register message handler
    gMessageManager().registerHandler<EndTurnMessage>(&HeadUpDisplay::onEndTurn, this);
}

void HeadUpDisplay::update(gf::Time time) {
    m_turnTime.addTo(time);
}

void HeadUpDisplay::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coordinates(target);

    gf::Text text;
    text.setFont(m_font);
    text.setOutlineColor(gf::Color::White);
    text.setOutlineThickness(2.0f);
    text.setCharacterSize(coordinates.getRelativeCharacterSize(0.05f));

    // Compute the time after the next turn
    int seconds = 3 - std::ceil(m_turnTime.asSeconds());
    if (seconds < 0) {
        seconds = 0;
    }

    text.setString("Next turn in " + std::to_string(seconds) + " sec");

    switch (m_currentPlayer) {
    case 1:
        text.setColor(gf::Color::fromRgba32(0x8f, 0x0e, 0x01));
        text.setAnchor(gf::Anchor::TopLeft);
        text.setPosition(coordinates.getRelativePoint({ 0.025f, 0.025f }));
        break;
    case 2:
        text.setColor(gf::Color::fromRgba32(0x00, 0x53, 0x8c));
        text.setAnchor(gf::Anchor::TopRight);
        text.setPosition(coordinates.getRelativePoint({ 0.975f, 0.025f }));
        break;
    default:
        assert(false);
    }


    target.draw(text, states);
}

gf::MessageStatus HeadUpDisplay::onEndTurn(gf::Id id, gf::Message *msg) {
    assert(id == EndTurnMessage::type);
    EndTurnMessage *endTurn = reinterpret_cast<EndTurnMessage*>(msg);

    m_currentPlayer = endTurn->playerID;
    m_turnTime = gf::Time::zero();

    return gf::MessageStatus::Keep;
}
