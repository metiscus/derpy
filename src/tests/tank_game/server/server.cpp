#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <vector>
#include <algorithm>

#include "Api.h"
#include "GameSession.h"
#include "Logging.h"
#include "Network.h"
#include "TankMap.h"

enum State {
	Invalid,
	Waiting,
	Generating,
	Playing
};

const static int listenPort = 31415;
const static size_t sessionPlayerCount = 2;

int main ( )
{
	int userId = 1000;
	std::vector<GameSession*> sessions;
	std::vector<ClientConnection<Message>*> pendingConnections;

	// open listen socket
	Debug("Starting server on port %d", listenPort);
	int serverSocket = streamServer(listenPort);

	
	// main loop
	for(;;)
	{
		Debug("...waiting for connections...");

		// listen for new connections
		int newSockFd = accept( serverSocket, NULL, 0 );
		if( newSockFd != -1 )
		{
			setBlocking(newSockFd, false);
			Debug("New client connected.");
			ClientConnection<Message> *newClient = new ClientConnection<Message>(userId++, newSockFd);
			pendingConnections.push_back(newClient);
		}

		// check sessions for timeout
		for(size_t connIdx=0; connIdx<pendingConnections.size(); ++connIdx)
		{
			while(pendingConnections[connIdx]->receiveMessages()) { ; }
			if(pendingConnections[connIdx]->getState() == ClientConnection<Message>::Disconnected)
			{
				ClientConnection<Message>* ptr = pendingConnections[connIdx];
				delete ptr;

				pendingConnections.erase( std::remove( pendingConnections.begin(), pendingConnections.end(), ptr), pendingConnections.end() );
				connIdx -= 1; 

				Debug("Pending client timed out. Connection ended.");
			}
		}

		// create sessions
		while( pendingConnections.size() >= sessionPlayerCount )
		{
			Debug("Creating a new session");
			GameSession *newSession = new GameSession();
			newSession->addClient(pendingConnections[0]);
			newSession->addClient(pendingConnections[1]);
			newSession->begin();
			sessions.push_back(newSession);

			// copy / reduce
			std::vector<ClientConnection<Message>*> tmp(pendingConnections.begin() + 2, pendingConnections.end());
			pendingConnections = tmp;
		}

		// check for completed sessions
		for(size_t sessIdx=0; sessIdx<sessions.size(); ++sessIdx)
		{
			if(sessions[sessIdx]->getState() == GameSession::Ended)
			{
				delete sessions[sessIdx];

				sessions.erase( std::remove( sessions.begin(), sessions.end(), sessions[sessIdx]), sessions.end() );
				sessIdx -= 1; 
			}
		}

		usleep(10000);
	}

	return 0;
}
