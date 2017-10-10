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

#ifndef _LOCAL_CONTROLLER_MANAGER_H
#define _LOCAL_CONTROLLER_MANAGER_H

#include <functional>
#include <map>
#include <memory>
#include <string>

#include <gf/Action.h>

using ActionCallback = std::function<void(void)>;

class ControlHandler {
public:
    ControlHandler(const std::string &name, const ActionCallback &callback);

    gf::Action& getAction();
    void processCallback();

private:
    gf::Action m_action;
    ActionCallback m_callback;
};

class ControllerManager {
public:
    void addControlHandler(ControlHandler &controlHandler);

    void processEvent(const gf::Event& event);
    void processActions();
    void reset();

private:
    std::map<std::string, ControlHandler*> m_controls;
};

#endif // _LOCAL_CONTROLLER_MANAGER_H
