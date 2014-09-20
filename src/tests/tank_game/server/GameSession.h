#ifndef GAMESESSION_H_
#define GAMESESSION_H_

#include "Api.h"
#include "Session.h"
#include "TankMap.h"
#include "ClientConnection.h"

class GameSession : public Session< ClientConnection<Message> >
{
public:
	GameSession();
	virtual ~GameSession();
	virtual void begin();
	virtual void end();
	virtual void run();

private:
	TankMap mTankMap;
};

#endif 