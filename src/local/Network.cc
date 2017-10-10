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

#include <cstdio>
#include <assert.h>
#include "Network.h"

//################################# HOST #######################################

Host::Host() {
}

void Host::createConnection(){
    printf("Server : Waiting for Client ... \n");
    // link listener to a port
    if (m_listener.listen(9000) != sf::Socket::Done) {
        printf("Error with port number\n");
        assert(false);
    }
    // accept new connection

    if (m_listener.accept(m_server) != sf::Socket::Done) {
        printf("Client not found\n");
        assert(false);
    }
    printf("A client has connected to the server\n");
}

void Host::sendDirection(char dir) {
    m_data[0] = dir;
    if (m_server.send(m_data, 2) != sf::Socket::Done) {
        printf("Data could not be sent\n");
        assert(false);
    }
}

char Host::receivedDirection(){
    size_t received;
    if (m_server.receive(m_data, 2, received) != sf::Socket::Done) {
        printf("Data could not found\n");
    }
    return m_data[0];
}

//################################CHALLENGER####################################

Challenger::Challenger() {
}

void Challenger::sendDirection(char dir) {
    m_data[0] = dir;
    if (m_client.send(m_data, 2) != sf::Socket::Done) {
        printf("Data could not be sent\n");
        assert(false);
    }
}

char Challenger::receivedDirection(){
    size_t received;
    if (m_client.receive(m_data, 2, received) != sf::Socket::Done) {
        printf("Data could not found\n");
        assert(false);
    }
    return m_data[0];
}

void Challenger::createConnection() {
    sf::Socket::Status status = m_client.connect("127.0.0.1", 9000);
    if (status != sf::Socket::Done) {
        printf("Server not found\n");
        assert(false);
    }
    printf("Connected to server\n");
}
