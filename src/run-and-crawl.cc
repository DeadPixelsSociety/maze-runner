/*
 * Run and Crawl - A student indie game
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

#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/Log.h>
#include <gf/RenderWindow.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

#include "local/Constants.h"
#include "local/Player.h"

int main() {
    // Create the window
    gf::Window window("Run and Crawl", ScreenSize);
    gf::RenderWindow renderer(window);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    // Set level log
    gf::Log::setLevel(gf::Log::Debug);

    // Initialization entities
    gf::EntityContainer mainEntities;

    // Set the player
    Player player1(WorldCenter);
    mainEntities.addEntity(player1);

    // Add cameras
    gf::ViewContainer views;

    gf::ExtendView mainView;
    mainView.setSize(WorldSize);
    mainView.setCenter({ WorldSize.width * 0.5f, WorldSize.height * 0.5f });
    // mainView.setCenter(0.5f * WorldBounds.width); // Not compile, why?
    views.addView(mainView);

    views.setInitialScreenSize(ScreenSize);

    // Add actions
    gf::ActionContainer actions;

    gf::Action closeWindowAction("Close window");
    closeWindowAction.addCloseControl();
    closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
    actions.addAction(closeWindowAction);

    // Game loop
    gf::Clock clock;

    renderer.clear(gf::Color::White);

    while (window.isOpen()) {
        // Input
        gf::Event event;
        while (window.pollEvent(event)) {
            actions.processEvent(event);
            views.processEvent(event);
        }

        if (closeWindowAction.isActive()) {
            window.close();
        }

        gf::Time time = clock.restart();

        // Update
        mainEntities.update(time);

        // Render
        renderer.clear();

        renderer.setView(mainView);
        mainEntities.render(renderer);

        renderer.display();
    }

    return 0;
}
