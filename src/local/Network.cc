#include <cstdio>
#include <assert.h>
#include "Network.h"

//################################# HOST #######################################

Host::Host() {
}

void Host::createConnection(){
  printf("Server \n");
  // link listener to a port
  if (m_listener.listen(9000) != sf::Socket::Done) {
    printf("Error with port number\n");
    assert(true);
  }
  // accept new connection

  if (m_listener.accept(m_server) != sf::Socket::Done) {
    printf("Client not found\n");
    assert(true);
  }
  printf("A client has connected to the server\n");
}

int Host::sendDirection(char dir) {
  m_data[0] = dir;
  if (m_server.send(m_data, 2) != sf::Socket::Done) {
    printf("Data could not be sent\n");
    return 1;
  }
  return 0;
}

char Host::receivedDirection(){
  size_t received;
  if (m_server.receive(m_data, 2, received) != sf::Socket::Done) {
    printf("Data could not found\n");
  }
}

//################################CHALLENGER####################################

Challenger::Challenger() {
}

int Challenger::sendDirection(char dir) {
  m_data[0] = dir;
  if (m_client.send(m_data, 2) != sf::Socket::Done) {
    printf("Data could not be sent\n");
    assert(true);
  }
  return 0;
}

char Challenger::receivedDirection(){
  size_t received;
  if (m_client.receive(m_data, 2, received) != sf::Socket::Done) {
    printf("Data could not found\n");
    assert(true);
  }
  return m_data[0];
}

void Challenger::createConnection() {
  printf("Client \n");
  sf::Socket::Status status = m_client.connect("127.0.0.1", 9000);

  if (status != sf::Socket::Done) {
    printf("Server not found\n");
    assert(true);
  }
  printf("Connected to server\n");
}

























/*
char receivedDirection(int status){
  size_t received;
  if(status==1){
    if (server.receive(data, 2, received) != sf::Socket::Done) {
      printf("Data could not found\n");
    }
  } else {
    if (server.receive(data, 2, received) != sf::Socket::Done) {
      printf("Data could not found\n");
    }
  }
  return data[0];
}
 */