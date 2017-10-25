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

#include "Network.h"

//################################# HOST #######################################

Host::Host() {
}

void Host::createConnection(){
    gf::Log::info("Server : Waiting for Client ... \n");
    // link listener to a port
    if (m_listener.listen(9000) != sf::Socket::Done) {
        gf::Log::error("Error with port number\n");
        assert(false);
    }
    // accept new connection

    if (m_listener.accept(m_server) != sf::Socket::Done) {
        gf::Log::error("Client not found\n");
        assert(false);
    }
    gf::Log::info("A client has connected to the server\n");
}

void Host::sendDirection(gf::Direction action) {
    m_action = action;
    if (m_server.send(&m_action, 1) != sf::Socket::Done) {
        gf::Log::error("Data could not be sent\n");
        assert(false);
    }
}

gf::Direction Host::receivedDirection(){
    size_t received;
    if (m_server.receive(&m_action, 2, received) != sf::Socket::Done) {
        gf::Log::error("Data could not found\n");
    }
    return m_action;
}

//################################CHALLENGER####################################

Challenger::Challenger() {
}

void Challenger::sendDirection(gf::Direction action) {
    m_action = action;
    if (m_client.send(&m_action, 1) != sf::Socket::Done) {
        gf::Log::error("Data could not be sent\n");
        assert(false);
    }
}

gf::Direction Challenger::receivedDirection(){
    size_t received;
    if (m_client.receive(&m_action, 1, received) != sf::Socket::Done) {
        gf::Log::error("Data could not found\n");
        assert(false);
    }
    return m_action;
}

void Challenger::createConnection(std::string IPAddress) {
    //TODO modify IP address
    sf::Socket::Status status = m_client.connect(IPAddress, 9000);
    if (status != sf::Socket::Done) {
        gf::Log::error("Server not found\n");
        assert(false);
    }
    gf::Log::info("Connected to server\n");
}
