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

#ifndef _HEAD_UP_DISPLAY_LOCAL_H
#define _HEAD_UP_DISPLAY_LOCAL_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/Time.h>

#include "Messages.h"

class HeadUpDisplay: public gf::Entity {
public:
    HeadUpDisplay();

    virtual void update(gf::Time time) override;
    virtual void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

    gf::MessageStatus onEndTurn(gf::Id id, gf::Message *msg);

private:
    gf::Font &m_font;
    gf::Time m_turnTime;
};

#endif  // _HEAD_UP_DISPLAY_LOCAL_H
