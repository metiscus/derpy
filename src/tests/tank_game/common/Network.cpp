#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "Network.h"
#include "Logging.h"

void setBlocking(Socket sock, bool block)
{
	int flags = fcntl(sock, F_GETFL, 0);
   	if (flags < 0)
   	{
   		Error("fcntl failed reading flags");
   	}
   	else {
   		if(block)
   		{
   			flags &= (~O_NONBLOCK);
   		}
   		else {
   			flags |= O_NONBLOCK;
   		}

		flags = fcntl(sock, F_SETFL, flags);
		if(flags < 0)
		{
			Error("fcntl failed setting flags");
		}
   	}
}

Socket streamServer(int port)
{
	Trace("Opening server socket on port %d.", port);
	Socket sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock == -1)
	{
		Error("Unable to open a new socket.");
	}
	else {
		struct sockaddr_in saddr;
		memset(&saddr, 0, sizeof(sockaddr_in));
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(port);
		saddr.sin_addr.s_addr = htonl(INADDR_ANY);
		if(bind(sock, (const sockaddr*)&saddr, sizeof(sockaddr_in))==-1)
		{
			close(sock);
			sock = -1;
			Error("Failed to bind to port %d.", port);
		}
		else {
			if(listen(sock, 1) == -1)
			{
				close(sock);
				sock = -1;
				Error("Listen failed.");
			}
		}
	}

	return sock;
}

Socket streamClient(char* host, int port, bool keepTrying)
{
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock == -1)
	{
		Error("Unable to open a new socket.");
	}
	else {
		struct sockaddr_in saddr;
		memset(&saddr, 0, sizeof(sockaddr_in));
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(port);
		saddr.sin_addr.s_addr = inet_addr(host);

		bool isConnected = false;

		do
		{
			if(connect(sock, (struct sockaddr *)&saddr, sizeof(saddr)) ==-1)
			{
				Trace("Failed to connect to %s:%d.", host, port);
				usleep(100000);
			}
			else {
				isConnected = true;
			}
		} while( !isConnected && keepTrying );

		if(!isConnected)
		{
			close(sock);
			sock = -1;
		}
	}

	return sock;
}