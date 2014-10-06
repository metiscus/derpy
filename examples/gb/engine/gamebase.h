#ifndef GAMEBASE_H_
#define GAMEBASE_H_

#include "gameclock.h"

class GameInputBase;

class GameBase
{
public:
	GameBase();
	virtual ~GameBase();

	virtual void configure(int argc, const char** argv) = 0;
	virtual void run();
	virtual void pause();
	virtual void unpause();
	virtual void stop();
	virtual void processInput(GameInputBase* input) = 0;

protected:
	virtual void preframe() = 0;
	virtual void frame() = 0;
	virtual void postframe() = 0;


	const GameClock& getClock() const;
	GameClock& getClock();

private:
	GameClock mClock;
	bool mIsRunning;
	bool mIsPaused;
};

#endif //GAMEBASE_H_