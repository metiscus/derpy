#include "TankMap.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <vector>
#include <algorithm>

#include "TankMap.h"

#include "api.h"

void setNonblock(int sock);
int startServer(int port);

struct MessageHeader;

enum State {
	Invalid,
	Waiting,
	Generating,
	Playing
};

int serverSocket = -1;

void cleanup()
{
	close(serverSocket);
}

int main ( )
{
	const int listenPort = 31415;
	std::vector<int> clients;
	std::vector<MessageHeader*> messages;

	TankMapParams tmp;
	tmp.width = 250;
	tmp.max_height = 300;
	tmp.min_height = 3;
	tmp.variance = 10000;
	tmp.mean = 150;
	tmp.smoothing_passes = 40;
	tmp.smoothing_coeff = 0.24;

	TankMap tm;

	SetActionMsg action_messages[2];
	int needAction = 2;

	atexit(cleanup);

	int *derp = new int;

	State state = Invalid;
	for(;;)
	{
		switch(state)
		{
			case Invalid:
			{
				serverSocket = startServer(listenPort);
				if(serverSocket != -1)
				{
					state = Waiting;
				}
				break;
			}

			case Waiting:
			{
				fd_set fdread, fderror;
				FD_ZERO(&fdread);
				FD_ZERO(&fderror);
				
				// add the server to the read and error queues
				FD_SET(serverSocket, &fdread);
				FD_SET(serverSocket, &fderror);

				for(unsigned int ii=0; ii<clients.size(); ++ii)
				{
					FD_SET(clients[ii], &fderror);
				}

				struct timeval tv;
				tv.tv_sec  = 1;
				tv.tv_usec = 0;

				int maxFd = serverSocket;
				for(int ii=0; ii<clients.size(); ++ii)
				{
					if(clients[ii]>maxFd)
					{
						maxFd = clients[ii];
					}
				}

				int returns = select(maxFd + 1, &fdread, NULL, &fderror, &tv);

				printf("%d clients currently connected. Waiting for connections...\n", clients.size());

				// check the special socket first
				if(FD_ISSET(serverSocket, &fdread)||FD_ISSET(serverSocket, &fderror))
				{
					int newClientSock = accept(serverSocket, NULL, 0);
					if(newClientSock != -1)
					{
						printf("new client connected...\n");
						setNonblock(newClientSock);
						clients.push_back(newClientSock);
					}

					FD_CLR(serverSocket, &fdread); 
				}

				// now process all the sockets
				for(int ii=0; ii<clients.size(); ++ii)
				{
					if(FD_ISSET(clients[ii], &fderror))
					{
						printf("client disconnected...\n");
						shutdown(clients[ii], SHUT_RDWR);

						clients.erase(std::remove(clients.begin(), clients.end(), clients[ii]), clients.end());
					}
				}

				if(clients.size() == 2)
				{
					printf("enough clients have joined for us to start...\n");
					state = Generating;
				}

				break;
			}

			case Generating:
			{
				tm.generate((long long)((void*)derp), tmp);

				printf("sending map...\n");
				// send the map to the connected clients
				HeresMapMsg *heresMapMsg = build_HeresMapMsg(tmp.width, 10, tmp.width - 10, (char*)tm.getHeights(), tmp.width * sizeof(float));
				if(!heresMapMsg)
				{
					printf("failed to make map message\n");
					exit(-1);
				}

				for(int ii=0; ii<clients.size(); ++ii)
				{
					heresMapMsg->youare = ii;
					if( send(clients[ii], heresMapMsg, heresMapMsg->hdr.length, 0) != heresMapMsg->hdr.length )
					{
						printf("failed to send map.\n");
					}
				}

				free(heresMapMsg);

				state = Playing;

				break;
			}


			case Playing:
			{
				fd_set fdread, fderror;
				FD_ZERO(&fdread);
				FD_ZERO(&fderror);
				
				for(int ii=0; ii<clients.size(); ++ii)
				{
					FD_SET(clients[ii], &fdread);
					FD_SET(clients[ii], &fderror);
				}

				struct timeval tv;
				tv.tv_sec  = 1;
				tv.tv_usec = 0;

				int maxFd = 0;
				for(int ii=0; ii<clients.size(); ++ii)
				{
					if(clients[ii]>maxFd)
					{
						maxFd = clients[ii];
					}
				}

				printf("waiting on round actions...\n");

				int returns = select(maxFd + 1, &fdread, NULL, &fderror, &tv);

				if(returns == 0)
				{
					continue;
				}

				for(int ii=0; ii<clients.size(); ++ii)
				{
					if (FD_ISSET(clients[ii], &fderror))
					{
						printf("client disconnect. session over\n");
						for(int ii=0; ii<clients.size(); ++ii)
						{
							shutdown(clients[ii], SHUT_RDWR);
						}

						clients.clear();
						state = Waiting;
						continue;
					}

					if(FD_ISSET(clients[ii], &fdread))
					{
						int rc = recv(clients[ii], (void*)&action_messages[ii], sizeof(SetActionMsg), 0);
						if(rc == sizeof(SetActionMsg) && action_messages[ii].hdr.type == SetAction)
						{
							--needAction;
						}
						else {
							shutdown(clients[ii], SHUT_RDWR);
						}
					}
				}

				if(needAction == 0)
				{
					printf("all actions in... do round!\n");
					DoRoundMsg msg;
					msg.hdr.type = DoRound;
					msg.hdr.length = sizeof(DoRoundMsg);
					msg.player_1_angle = action_messages[0].angle;
					msg.player_1_power = action_messages[0].power;
					msg.player_2_angle = action_messages[1].angle;
					msg.player_2_power = action_messages[1].power;

					for(int ii=0; ii<clients.size(); ++ii)
					{
						if(send(clients[ii], &msg, sizeof(msg), 0) == -1)
						{
							printf("send fail..\n");
							exit(1);
						}
					}

					needAction = 2;
				}

				break;
			}
		}
	}
}

int startServer(int port)
{
	printf("opening socket..\n");
	int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	printf("mb %d\n", serverSocket);
	if(serverSocket==-1)
	{
		printf("failed\n");
		exit(-1);
	}

	setNonblock(serverSocket);

	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(sockaddr_in));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(serverSocket, (const sockaddr*)&saddr, sizeof(sockaddr_in))==-1)
	{
		close(serverSocket);
		printf("bind failed\n");
		return -1;
	}

	if(listen(serverSocket, 1) == -1)
	{
		close(serverSocket);
		printf("listen failed\n");
		return -1;
	}

	return serverSocket;
}

void setNonblock(int sock)
{
	int flags = fcntl(sock, F_GETFL, 0);
   	if (flags < 0)
   	{
   		printf("fcntl failed\n");
   	}
   	else {
   		flags |= O_NONBLOCK;
		fcntl(sock, F_SETFL, flags);
   	}
}