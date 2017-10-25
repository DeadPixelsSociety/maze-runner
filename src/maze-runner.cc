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
    size_t status=2;
    Host host;
    Challenger challenger;
    if (argv[1][1] == 's') {
        host.createConnection();
        status = 0;//0 = Server
    } else if (argv[1][1] == 'c') {
        std::string IPAddress = argv[2];
        challenger.createConnection(IPAddress);
        status = 1;//1 = Client
    }
    assert(status != 2);

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

    player2.setEndTurn();
    while (window.isOpen()) {
        // Input
        gf::Event event;
        while (window.pollEvent(event)) {
            actions.processEvent(event);
            views.processEvent(event);
        }

        // Actions
        if (closeWindowAction.isActive()) {
            window.close();
        }

        if (status == 0) {
            //TODO refactor to Player.Update maybe, to be cleaner ?
            //gf::Log::info("%s\n", player1.hisTurn() ? "my turn" : "challenger turn");
            if(player1.hisTurn()){
                //gf::Log::info("Waiting for input\n");
                if (rightActionPlayer1.isActive()) {
                    player1.goTo(gf::Direction::Right);
                    player1.setEndTurn();
                    host.sendDirection(gf::Direction::Right);
                    gf::Log::info("Sending Right\n");
                } else if (leftActionPlayer1.isActive()) {
                    player1.goTo(gf::Direction::Left);
                    player1.setEndTurn();
                    host.sendDirection(gf::Direction::Left);
                    gf::Log::info("Sending Left\n");
                } else if (upActionPlayer1.isActive()) {
                    player1.goTo(gf::Direction::Up);
                    player1.setEndTurn();
                    host.sendDirection(gf::Direction::Up);
                    gf::Log::info("Sending Up\n");
                } else if (downActionPlayer1.isActive()) {
                    player1.goTo(gf::Direction::Down);
                    player1.setEndTurn();
                    host.sendDirection(gf::Direction::Down);
                    gf::Log::info("Sending Down\n");
                }
            } else {
                gf::Log::info("Waiting for opponent move\n");
                switch (host.receivedDirection()) {
                case gf::Direction::Right :
                    player2.goTo(gf::Direction::Right);
                    player2.setEndTurn();
                    break;
                case gf::Direction::Left :
                    player2.goTo(gf::Direction::Left);
                    player2.setEndTurn();
                    break;
                case gf::Direction::Up :
                    player2.goTo(gf::Direction::Up);
                    player2.setEndTurn();
                    break;
                case gf::Direction::Down :
                    player2.goTo(gf::Direction::Down);
                    player2.setEndTurn();
                    break;
                default:
                    assert(false);
                }
            }
        } else {
            //gf::Log::info("%s\n", player2.hisTurn() ? "my turn" : "host turn");
            if(player2.hisTurn()){
                // Actions for player 2
                //gf::Log::info("Waiting for input\n");
                if (rightActionPlayer2.isActive()) {
                  player2.goTo(gf::Direction::Right);
                  player2.setEndTurn();
                  challenger.sendDirection(gf::Direction::Right);
                } else if (leftActionPlayer2.isActive()) {
                  player2.goTo(gf::Direction::Left);
                  player2.setEndTurn();
                  challenger.sendDirection(gf::Direction::Left);
                } else if (upActionPlayer2.isActive()) {
                  player2.goTo(gf::Direction::Up);
                  player2.setEndTurn();
                  challenger.sendDirection(gf::Direction::Up);
                } else if (downActionPlayer2.isActive()) {
                  player2.goTo(gf::Direction::Down);
                  player2.setEndTurn();
                  challenger.sendDirection(gf::Direction::Down);
              }
            } else {
                gf::Log::info("Waiting for opponent move\n");
                // Actions for player 1
                switch (challenger.receivedDirection()) {
                case gf::Direction::Right :
                    player1.goTo(gf::Direction::Right);
                    player1.setEndTurn();
                    break;
                case gf::Direction::Left :
                    player1.goTo(gf::Direction::Left);
                    player1.setEndTurn();
                    break;
                case gf::Direction::Up :
                    player1.goTo(gf::Direction::Up);
                    player1.setEndTurn();
                    break;
                case gf::Direction::Down :
                    player1.goTo(gf::Direction::Down);
                    player1.setEndTurn();
                    break;
                default:
                    assert(false);
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
