#include <cstdio>
#include "Network.h"

int Server(){
  printf("Server \n");
  // link listener to a port
  if (listener.listen(9000) != sf::Socket::Done) {
    printf("Error with port number\n");
    return 2;
  }
  // accept new connection

  if (listener.accept(server) != sf::Socket::Done) {
    printf("Client not found\n");
    return 1;
  }
  printf("A client has connected to the server\n");
  return 0;
}

int Client() {
  printf("Client \n");
  sf::Socket::Status status = client.connect("127.0.0.1", 9000);

  if (status != sf::Socket::Done) {
    printf("Server not found\n");
    return 1;
  }
  printf("Connected to server\n");
  return 0;
}

int sendDirection(int status, char dir){
  data[0] = dir;
  if(status==1){
    if (client.send(data, 2) != sf::Socket::Done) {
      printf("Data could not be sent\n");
      return 1;
    }
    return 0;
  } else {
    if (server.send(data, 2) != sf::Socket::Done) {
      printf("Data could not be sent\n");
      return 1;
    }
    return 0;
  }
}

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