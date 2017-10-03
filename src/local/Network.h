#ifndef _NETWORK_LOCAL_H
#define _NETWORK_LOCAL_H

#include <SFML/Network.hpp>

class Host{
private:
  sf::TcpListener m_listener;
  sf::TcpSocket m_server;
  char m_data[2];
public :
  Host();
  void sendDirection(char dir);
  char receivedDirection();
  void createConnection();
};

class Challenger{
private:
  sf::TcpSocket m_client;
  char m_data[2];
public:
  Challenger();
  void sendDirection(char dir);
  char receivedDirection();
  void createConnection();
};

#endif //_NETWORK_LOCAL_H
