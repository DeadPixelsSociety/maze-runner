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
#include "local/Map.h"
#include "local/Network.h"
#include "local/Player.h"
#include "local/Singletons.h"

#include "config.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        gf::Log::error("Need one Parameter\n");
        gf::Log::error("Use -c for client\nUse -s for Server\n");
        return 1;
    }
    int status;
    Host host;
    Challenger challenger;
    if (argv[1][1] == 's') {
        host.createConnection();
        status = 0;//0 = Server
    } else if (argv[1][1] == 'c') {
        challenger.createConnection();
        status = 1;//1 = Client
    }

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

    gf::SingletonStorage<gf::MessageManager> storageForMessageManager(gMessageManager);

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
    mainView.setCenter({WorldSize.width * 0.5f, WorldSize.height * 0.5f});
    // mainView.setCenter(0.5f * WorldBounds); // Not compile, why?
    views.addView(mainView);

    views.setInitialScreenSize(ScreenSize);

    // Add actions
    gf::ActionContainer actions;

    gf::Action closeWindowAction("Close window");
    closeWindowAction.addCloseControl();
    closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
    actions.addAction(closeWindowAction);

    gf::Action leftActionPlayer1("Player 1 - Left");
    gf::Action rightActionPlayer1("Player 1 - Right");
    gf::Action upActionPlayer1("Player 1 - Up");
    gf::Action downActionPlayer1("Player 1 - Down");

    gf::Action leftActionPlayer2("Player 2 - Left");
    gf::Action rightActionPlayer2("Player 2 - Right");
    gf::Action upActionPlayer2("Player 2 - Up");
    gf::Action downActionPlayer2("Player 2 - Down");

    if (status == 0) {
        // Actions for player 1
        leftActionPlayer1.addScancodeKeyControl(gf::Scancode::A);
        // leftActionPlayer1.setContinuous(); // later
        actions.addAction(leftActionPlayer1);

        rightActionPlayer1.addScancodeKeyControl(gf::Scancode::D);
        // rightActionPlayer1.setContinuous(); // later
        actions.addAction(rightActionPlayer1);

        upActionPlayer1.addScancodeKeyControl(gf::Scancode::W);
        // upActionPlayer1.setContinuous(); // later
        actions.addAction(upActionPlayer1);

        downActionPlayer1.addScancodeKeyControl(gf::Scancode::S);
        // downActionPlayer1.setContinuous(); // later
        actions.addAction(downActionPlayer1);
    } else {
        leftActionPlayer2.addScancodeKeyControl(gf::Scancode::A);
        // leftActionPlayer2.setContinuous(); // later
        actions.addAction(leftActionPlayer2);

        rightActionPlayer2.addScancodeKeyControl(gf::Scancode::D);
        // rightActionPlayer2.setContinuous(); // later
        actions.addAction(rightActionPlayer2);

        upActionPlayer2.addScancodeKeyControl(gf::Scancode::W);
        // upActionPlayer2.setContinuous(); // later
        actions.addAction(upActionPlayer2);

        downActionPlayer2.addScancodeKeyControl(gf::Scancode::S);
        // downActionPlayer2.setContinuous(); // later
        actions.addAction(downActionPlayer2);
    }
    // Dirty: set the initial turn
    EndTurnMessage msg;
    msg.player = &player2;
    gMessageManager().sendMessage(&msg);

    // Game loop
    gf::Clock clock;

    renderer.clear(gf::Color::White);

    bool hostTurn = true;

    while (window.isOpen()) {
        // Input
        gf::Event event;
        char dir;
        while (window.pollEvent(event)) {
            actions.processEvent(event);
            views.processEvent(event);
        }

        // Actions
        if (closeWindowAction.isActive()) {
            window.close();
        }
        if (status == 0) {
            //TODO change token base turn to time based turn with messages implementation like master
            gf::Log::info("%s\n", hostTurn ? "my turn" : "challenger turn");
            if(hostTurn){
                gf::Log::info("Waiting for input\n");
                if (rightActionPlayer1.isActive()) {
                    player1.goTo(gf::Direction::Right);
                    hostTurn=false;
                    host.sendDirection('R');
                    gf::Log::info("Sending Right\n");
                } else if (leftActionPlayer1.isActive()) {
                  player1.goTo(gf::Direction::Left);
                    hostTurn=false;
                    host.sendDirection('L');
                    gf::Log::info("Sending Left\n");
                } else if (upActionPlayer1.isActive()) {
                  player1.goTo(gf::Direction::Up);
                    hostTurn=false;
                    host.sendDirection('U');
                    gf::Log::info("Sending Up\n");
                } else if (downActionPlayer1.isActive()) {
                  player1.goTo(gf::Direction::Down);
                    hostTurn=false;
                    host.sendDirection('D');
                    gf::Log::info("Sending Down\n");
                }
            } else {
                gf::Log::info("Waiting for opponent move\n");
                dir = host.receivedDirection();
                gf::Log::info("%c\n", dir);
                if (dir == 'R') {
                    player2.goTo(gf::Direction::Right);
                    hostTurn=true;
                } else if (dir == 'L') {
                    player2.goTo(gf::Direction::Left);
                    hostTurn=true;
                } else if (dir == 'U') {
                    player2.goTo(gf::Direction::Up);
                    hostTurn=true;
                } else if (dir == 'D') {
                    player2.goTo(gf::Direction::Down);
                    hostTurn=true;
                }
            }
        } else {
            gf::Log::info("%s\n", hostTurn ? "host turn" : "my turn");
            if(hostTurn){
                gf::Log::info("Waiting for opponent move\n");
                dir = challenger.receivedDirection();
                gf::Log::info("%c\n", dir);
                // Actions for player 1
                if (dir == 'R') {
                    player1.goTo(gf::Direction::Right);
                    hostTurn=false;
                } else if (dir == 'L') {
                    player1.goTo(gf::Direction::Left);
                    hostTurn=false;
                } else if (dir == 'U') {
                    player1.goTo(gf::Direction::Up);
                    hostTurn=false;
                } else if (dir == 'D') {
                    player1.goTo(gf::Direction::Down);
                    hostTurn=false;
                }
            } else {
                // Actions for player 2
                gf::Log::info("Waiting for input\n");
                if (rightActionPlayer2.isActive()) {
                    player2.goTo(gf::Direction::Right);
                    hostTurn=true;
                    challenger.sendDirection('R');
                } else if (leftActionPlayer2.isActive()) {
                    player2.goTo(gf::Direction::Left);
                    hostTurn=true;
                    challenger.sendDirection('L');
                } else if (upActionPlayer2.isActive()) {
                    player2.goTo(gf::Direction::Up);
                    hostTurn=true;
                    challenger.sendDirection('U');
                } else if (downActionPlayer2.isActive()) {
                    player2.goTo(gf::Direction::Down);
                    hostTurn=true;
                    challenger.sendDirection('D');
                }
            }
        }

        gf::Time time = clock.restart();

        // Update
        mainEntities.update(time);

        // Render
        renderer.clear();

        renderer.setView(mainView);
        mainEntities.render(renderer);

        renderer.display();

        actions.reset();
    }

    return 0;
}
