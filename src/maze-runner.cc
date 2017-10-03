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
#include "local/Map.h"
#include "local/Network.h"
#include "local/Player.h"


int main(int argc, char *argv[]) {
    if(argc<2){
        printf("Need one Parameter\n");
        printf("Use -c for client\nUse -s for Server\n");
        return 1;
    }
    int status;
    if(argv[1][1]=='s'){
      //TODO recover error code
        Server();
        status=0;//0 = Server
    } else if (argv[1][1]=='c') {
      //TODO recover error code
        Client();
        status=1;//1 = Client
    } else if (argv[1][1]=='h') {
        printf("Use -c for client\nUse -s for Server\n");
        return 0;
    } else {
        printf("Use -c for client\nUse -s for Server\n");
        return 1;
    }

    // Create the window
    gf::Window window("Maze Runner", ScreenSize);
    gf::RenderWindow renderer(window);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    // Set level log
    gf::Log::setLevel(gf::Log::Debug);

    // Initialization entities
    gf::EntityContainer mainEntities;

    // Set the player 1
    Player player1({ 0, WorldCenter.y });
    mainEntities.addEntity(player1);

    // Set the player 2
    Player player2({ WorldBounds.x - 1, WorldCenter.y });
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
    gf::ActionContainer actions;

    gf::Action closeWindowAction("Close window");
    closeWindowAction.addCloseControl();
    closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
    actions.addAction(closeWindowAction);

    // Actions for player 1
    gf::Action leftActionPlayer1("Player 1 - Left");
    leftActionPlayer1.addScancodeKeyControl(gf::Scancode::A);
    // leftActionPlayer1.setContinuous(); // later
    actions.addAction(leftActionPlayer1);

    gf::Action rightActionPlayer1("Player 1 - Right");
    rightActionPlayer1.addScancodeKeyControl(gf::Scancode::D);
    // rightActionPlayer1.setContinuous(); // later
    actions.addAction(rightActionPlayer1);

    gf::Action upActionPlayer1("Player 1 - Up");
    upActionPlayer1.addScancodeKeyControl(gf::Scancode::W);
    // upActionPlayer1.setContinuous(); // later
    actions.addAction(upActionPlayer1);

    gf::Action downActionPlayer1("Player 1 - Down");
    downActionPlayer1.addScancodeKeyControl(gf::Scancode::S);
    // downActionPlayer1.setContinuous(); // later
    actions.addAction(downActionPlayer1);

    gf::Action leftActionPlayer2("Player 2 - Left");
    leftActionPlayer2.addScancodeKeyControl(gf::Scancode::Left);
    // leftActionPlayer2.setContinuous(); // later
    actions.addAction(leftActionPlayer2);

    gf::Action rightActionPlayer2("Player 2 - Right");
    rightActionPlayer2.addScancodeKeyControl(gf::Scancode::Right);
    // rightActionPlayer2.setContinuous(); // later
    actions.addAction(rightActionPlayer2);

    gf::Action upActionPlayer2("Player 2 - Up");
    upActionPlayer2.addScancodeKeyControl(gf::Scancode::Up);
    // upActionPlayer2.setContinuous(); // later
    actions.addAction(upActionPlayer2);

    gf::Action downActionPlayer2("Player 2 - Down");
    downActionPlayer2.addScancodeKeyControl(gf::Scancode::Down);
    // downActionPlayer2.setContinuous(); // later
    actions.addAction(downActionPlayer2);

    // Game loop
    gf::Clock clock;

    renderer.clear(gf::Color::White);

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
        if(status==0){
          // Actions for player 1
          if (rightActionPlayer1.isActive()) {
            player1.goRight();
            sendDirection(0,'R');
          }
          else if (leftActionPlayer1.isActive()) {
            player1.goLeft();
            sendDirection(0,'L');
          }
          else if (upActionPlayer1.isActive()) {
            player1.goUp();
            sendDirection(0,'U');
          }
          else if (downActionPlayer1.isActive()) {
            player1.goDown();
            sendDirection(0,'D');
          }

          // Actions for player 2
          dir = receivedDirection(0);
          if (dir=='R') {
            player2.goRight();
          }
          else if (dir=='L') {
            player2.goLeft();
          }
          else if (dir=='U') {
            player2.goUp();
          }
          else if (dir=='D') {
            player2.goDown();
          }
        } else {
          dir=receivedDirection(1);
          // Actions for player 1
          if (dir=='R') {
            player1.goRight();
          }
          else if (dir=='L') {
            player1.goLeft();
          }
          else if (dir=='U') {
            player1.goUp();
          }
          else if (dir=='D') {
            player1.goDown();
          }

          // Actions for player 2
          if (rightActionPlayer2.isActive()) {
            player2.goRight();
            sendDirection(1,'R');
          }
          else if (leftActionPlayer2.isActive()) {
            player2.goLeft();
            sendDirection(1,'L');
          }
          else if (upActionPlayer2.isActive()) {
            player2.goUp();
            sendDirection(1,'U');
          }
          else if (downActionPlayer2.isActive()) {
            player2.goDown();
            sendDirection(1,'D');
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
