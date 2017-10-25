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

#ifndef _NETWORK_LOCAL_H
#define _NETWORK_LOCAL_H

#include <assert.h>
#include <cstdio>

#include <gf/Direction.h>
#include <gf/Log.h>

#include <SFML/Network.hpp>

//TODO overwrite gf:Direction to Action ?

class Host{
private:
  sf::TcpListener m_listener;
  sf::TcpSocket m_server;
  gf::Direction m_action;
public :
  Host();
  void sendDirection(gf::Direction action);
  gf::Direction receivedDirection();
  void createConnection();
};

class Challenger{
private:
  sf::TcpSocket m_client;
  gf::Direction m_action;
public:
  Challenger();
  void sendDirection(gf::Direction action);
  gf::Direction receivedDirection();
  void createConnection(std::string IPAddress);
};

#endif //_NETWORK_LOCAL_H
