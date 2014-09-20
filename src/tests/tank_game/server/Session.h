#ifndef SESSION_H_
#define SESSION_H_

#include "Network.h"
#include "Noncopyable.h"

#include <pthread.h>

#include <vector>

template<typename ConnectionType>
class Session : public noncopyable
{
public:
	enum State {
		Waiting,
		Running,
		Ended,
	};

	Session()
		: mState(Waiting)
	{
		;
	}

	virtual ~Session()
	{
		size_t idx;
		for(idx=0; idx<mClients.size(); ++idx)
		{
			delete (mClients[idx]);
		}

		mClients.clear();
	}

	void addClient(ConnectionType* ccon)
	{
		if(ccon)
		{
			mClients.push_back(ccon);
		}
	}

	State getState() const
	{
		return mState;
	}

	virtual void begin()
	{
		pthread_create(&mThread, 0, Session::delegate, this);
	}

	virtual void end()
	{
		pthread_cancel(mThread);
	}

	virtual void run() = 0;

protected:
	State mState;
	pthread_t mThread;
	std::vector<ConnectionType*> mClients;


	static void* delegate(void* ptr)
	{
		Session *pSession = (Session*)ptr;
		if(pSession)
		{
			pSession->mState = Running;
			pSession->run();
		}

		return 0;
	}

};

#endif //SESSION_H_