#ifndef CLIENTCONNECTION_H_
#define CLIENTCONNECTION_H_

#include <vector>

#include "Api.h"
#include "ClientConnection.h"
#include "Logging.h"
#include "Network.h"
#include "Noncopyable.h"

#include <errno.h>


template<typename MessageType>
class ClientConnection : public noncopyable
{
public:
	enum State {
		Connected,
		Disconnected,
	};

	ClientConnection(int id, Socket sock)
		: mId( id )
		, mSocket( sock )
		, mState( Connected )
	{
		;
	}

	~ClientConnection()
	{
		close(mSocket);
	}

	int getId() const
	{
		return mId;
	}

	State getState() const
	{
		return mState;
	}

	void sendMessage(MessageType* message)
	{
		if(message)
		{
			int rc = send(mSocket, (char*)message, sizeof(MessageType), 0);
			if( rc != sizeof(MessageType) )
			{
				Trace("Failed to send message");
			}
		}
	}

	bool receiveMessages()
	{
		MessageType *msg = new MessageType();
		int rc = recv(mSocket, (char*)&msg, sizeof(MessageType), MSG_DONTWAIT);
		if(rc==sizeof(MessageType))
		{
			mMessages.push_back(msg);
			return true;
		}
		else if(rc<=0) // an error occurred
		{
			if(rc==0)
			{
				mState = Disconnected;
			}

			switch(errno)
			{
				case EWOULDBLOCK:
				{
					return false;
					break;
				}

				default:
				{
					Trace("Remote disconnect.");
					mState = Disconnected;
					return false;
					break;
				}
			}
		}
		return false;
	}

	std::vector<MessageType*>& getMessages()
	{
		return mMessages;
	}

	Socket getSocket()
	{
		return mSocket;
	}

private:
	int         mId;
	Socket      mSocket;
	State       mState;

	std::vector<MessageType*> mMessages;
};

#endif //CLIENTCONNECTION_H_