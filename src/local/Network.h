#ifndef _NETWORK_LOCAL_H
#define _NETWORK_LOCAL_H

#include <SFML/Network.hpp>

sf::TcpListener listener;
sf::TcpSocket server;
sf::TcpSocket client;
char data[2];

int Server();

int Client();

int sendDirection(int status, char dir);

char receivedDirection(int status);

#endif //_NETWORK_LOCAL_H
