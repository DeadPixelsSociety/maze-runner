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

#ifndef _LOCAL_VIEWS_MANAGER_H
#define _LOCAL_VIEWS_MANAGER_H

#include <gf/Entity.h>
#include <gf/EntityContainer.h>
#include <gf/Font.h>
#include <gf/Views.h>

#include "local/Messages.h"
#include "local/Singletons.h"

class ViewManager : public gf::Entity {
public:
    ViewManager(gf::EntityContainer &mainEntities, gf::EntityContainer &hudEntities, gf::ExtendView &player1View, gf::ExtendView &player2View, gf::ScreenView &hudView);

    virtual void update(gf::Time time) override;
    void render(gf::RenderTarget &target);
    virtual void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

    gf::MessageStatus onGameOver(gf::Id id, gf::Message *msg);

private:
    void renderGameOverScreen(gf::RenderTarget &target, const gf::RenderStates &states);

private:
    enum State {
        InGame,
        GameOver,
    };

private:
    State m_currentState;
    gf::Font &m_font;

    gf::EntityContainer &m_mainEntities;
    gf::EntityContainer &m_hudEntities;

    gf::ExtendView &m_player1View;
    gf::ExtendView &m_player2View;
    gf::ScreenView &m_hudView;

    int m_numWinner;
};

#endif // _LOCAL_VIEWS_MANAGER_H
