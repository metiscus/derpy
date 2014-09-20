#include "GameSession.h"

GameSession::GameSession()
{
	;
}

GameSession::~GameSession()
{
	;
}

void GameSession::begin()
{
	// generate the map for this game session
	TankMapParams tmp;
	tmp.width = 250;
	tmp.max_height = 300;
	tmp.min_height = 3;
	tmp.variance = 10000;
	tmp.mean = 150;
	tmp.smoothing_passes = 40;
	tmp.smoothing_coeff = 0.24;
	
	mTankMap.generate((long long)((void*)this), tmp);

	for(size_t idx = 0; idx<mClients.size(); ++idx)
	{
		Message ssm;
		ssm.header.length = sizeof(Message);
		ssm.header.type   = StartSession;
		ssm.startSession.width = tmp.width;
		ssm.startSession.startX = (idx / 2.f * tmp.width / 10.f);
		if(idx % 2 == 0)
		{
			ssm.startSession.startX = tmp.width - ssm.startSession.startX;
		}
		memcpy(ssm.startSession.data, (char*)mTankMap.getHeights(), tmp.width * sizeof(float));

		mClients[idx]->sendMessage( &ssm );
		Trace("Sending start session message.");
	}

	Session< ClientConnection<Message> >::begin();
}

void GameSession::end()
{
	Session< ClientConnection<Message> >::end();
}

void GameSession::run()
{
	bool sessionRunning = true;

	while(sessionRunning)
	{
		// check sessions for timeout
		for(size_t connIdx=0; connIdx<mClients.size(); ++connIdx)
		{
			while(mClients[connIdx]->receiveMessages()) { Trace("..."); }
			if(mClients[connIdx]->getState() == ClientConnection<Message>::Disconnected)
			{
				Debug("Session client timed out. Connection ended.");
				sessionRunning = false;
			}
		}

		usleep(100000);
	}

	Debug("Session ended.");
}