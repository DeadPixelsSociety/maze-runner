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

#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/Log.h>
#include <gf/RenderWindow.h>
#include <gf/Singleton.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>

#include "local/Constants.h"
#include "local/ControllerManager.h"
#include "local/Map.h"
#include "local/Player.h"
#include "local/Singletons.h"

#include "config.h"

int main() {
    // Create the window
    gf::Window window("Maze Runner", ScreenSize);
    gf::RenderWindow renderer(window);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    // Set level log
    gf::Log::setLevel(gf::Log::Debug);

    // Set the singletons
    gf::SingletonStorage<gf::ResourceManager> storageForResourceManager(gResourceManager);
    gResourceManager().addSearchDir(MR_DATA_DIR);

    // Initialization entities
    gf::EntityContainer mainEntities;

    // Set the player 1
    Player player1(gf::Vector2i(1, WorldCenter.y));
    mainEntities.addEntity(player1);

    // Set the player 2
    Player player2(gf::Vector2i(WorldBounds.x - 2, WorldCenter.y));
    mainEntities.addEntity(player2);

    // Set the map
    Map map;
    mainEntities.addEntity(map);

    // Add cameras
    gf::ViewContainer views;

    gf::ExtendView mainView;
    mainView.setSize(WorldSize);
    mainView.setCenter({ WorldSize.width * 0.5f, WorldSize.height * 0.5f });
    // mainView.setCenter(0.5f * WorldBounds); // Not compile, why?
    views.addView(mainView);

    views.setInitialScreenSize(ScreenSize);

    // Add actions
    ControllerManager controller;

    // Close the window
    ControlHandler closeWindowControl("Close window", [&window]{window.close();});
    closeWindowControl.getAction().addCloseControl();
    closeWindowControl.getAction().addKeycodeKeyControl(gf::Keycode::Escape);
    controller.addControlHandler(closeWindowControl);

    // Actions for player 1
    ControlHandler leftControlPlayer1("Player 1 - Left", [&player1]{
        player1.goLeft();
    });
    leftControlPlayer1.getAction().addScancodeKeyControl(gf::Scancode::A);
    controller.addControlHandler(leftControlPlayer1);

    ControlHandler rightControlPlayer1("Player 1 - Right", [&player1]{
        player1.goRight();
    });
    rightControlPlayer1.getAction().addScancodeKeyControl(gf::Scancode::D);
    controller.addControlHandler(rightControlPlayer1);

    ControlHandler upControlPlayer1("Player 1 - Up", [&player1]{
        player1.goUp();
    });
    upControlPlayer1.getAction().addScancodeKeyControl(gf::Scancode::W);
    controller.addControlHandler(upControlPlayer1);

    ControlHandler downControlPlayer1("Player 1 - Down", [&player1]{
        player1.goDown();
    });
    downControlPlayer1.getAction().addScancodeKeyControl(gf::Scancode::S);
    controller.addControlHandler(downControlPlayer1);

    // Actions for player 2
    ControlHandler leftControlPlayer2("Player 2 - Left", [&player2]{
        player2.goLeft();
    });
    leftControlPlayer2.getAction().addScancodeKeyControl(gf::Scancode::Left);
    controller.addControlHandler(leftControlPlayer2);

    ControlHandler rightControlPlayer2("Player 2 - Right", [&player2]{
        player2.goRight();
    });
    rightControlPlayer2.getAction().addScancodeKeyControl(gf::Scancode::Right);
    controller.addControlHandler(rightControlPlayer2);

    ControlHandler upControlPlayer2("Player 2 - Up", [&player2]{
        player2.goUp();
    });
    upControlPlayer2.getAction().addScancodeKeyControl(gf::Scancode::Up);
    controller.addControlHandler(upControlPlayer2);

    ControlHandler downControlPlayer2("Player 2 - Down", [&player2]{
        player2.goDown();
    });
    downControlPlayer2.getAction().addScancodeKeyControl(gf::Scancode::Down);
    controller.addControlHandler(downControlPlayer2);

    // Game loop
    gf::Clock clock;

    renderer.clear(gf::Color::White);

    while (window.isOpen()) {
        // Input
        gf::Event event;
        while (window.pollEvent(event)) {
            controller.processEvent(event);
            views.processEvent(event);
        }

        // Actions
        controller.processActions();
        // if (closeWindowAction.isActive()) {
        //     window.close();
        // }
        //
        // // Actions for player 1
        // if (rightActionPlayer1.isActive()) {
        //     player1.goRight();
        // }
        // else if (leftActionPlayer1.isActive()) {
        //     player1.goLeft();
        // }
        // else if (upActionPlayer1.isActive()) {
        //     player1.goUp();
        // }
        // else if (downActionPlayer1.isActive()) {
        //     player1.goDown();
        // }
        //
        // // Actions for player 2
        // if (rightActionPlayer2.isActive()) {
        //     player2.goRight();
        // }
        // else if (leftActionPlayer2.isActive()) {
        //     player2.goLeft();
        // }
        // else if (upActionPlayer2.isActive()) {
        //     player2.goUp();
        // }
        // else if (downActionPlayer2.isActive()) {
        //     player2.goDown();
        // }

        gf::Time time = clock.restart();

        // Update
        mainEntities.update(time);

        // Render
        renderer.clear();

        renderer.setView(mainView);
        mainEntities.render(renderer);

        renderer.display();

        controller.reset();
    }

    return 0;
}
