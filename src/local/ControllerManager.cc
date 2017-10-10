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

#include "ControllerManager.h"

/* ====== ControlHandler ====== */
ControlHandler::ControlHandler(const std::string &name, const ActionCallback &callback) :
    m_action(name)
    , m_callback(callback) {
}

gf::Action& ControlHandler::getAction() {
    return m_action;
}

void ControlHandler::processCallback() {
    m_callback();
}

/* ====== ControllerManager ====== */
void ControllerManager::addControlHandler(ControlHandler &controlHandler) {
    m_controls.emplace(std::make_pair(controlHandler.getAction().getName(), &controlHandler));
}

void ControllerManager::processEvent(const gf::Event& event) {
    for (auto it = m_controls.begin(); it != m_controls.end(); ++it) {
        it->second->getAction().processEvent(event);
    }
}

void ControllerManager::processActions() {
    for (auto it = m_controls.begin(); it != m_controls.end(); ++it) {
        gf::Action &action = it->second->getAction();

        if (action.isActive()) {
            it->second->processCallback();
        }
    }
}

void ControllerManager::reset() {
    for (auto it = m_controls.begin(); it != m_controls.end(); ++it) {
        it->second->getAction().reset();
    }
}
