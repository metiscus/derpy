#ifndef NETWORK_H_
#define NETWORK_H_

#if defined(__linux)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#if defined(_WIN32)

#endif

typedef int Socket;

void setBlocking(Socket sock, bool block);

Socket streamServer(int port);
Socket streamClient(char* host, int port, bool keepTrying);

#endif //NETWORK_H_