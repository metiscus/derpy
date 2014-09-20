#include "Session.h"

#include "Logging.h"

Session::Session()
	: mState(Waiting)
{
	;
}

Session::~Session()
{
	std::vector<ClientConnection*> itr;
	for(itr = mClients.begin(); itr!=mClients.end(); ++itr)
	{
		delete (*mClient);
	}

	mClients.clear();
}

void Session::addClient(ClientConnection* ccon)
{
	if(ccon)
	{
		mClients.push_back(ccon);
	}
}

Session::State Session::getState() const
{
	return mState;
}

void* Session::delegate(void* ptr)
{
	Session *pSession = (Session*)ptr;
	if(pSession)
	{
		run();
	}
}

void Session::begin()
{
	pthread_create(&mThread, 0, Session::delegate, this);
}

void Session::end()
{
	pthread_cancel(&mThread);
}