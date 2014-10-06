#include "gamebase.h"

GameBase::GameBase()
	: mIsRunning(false)
	, mIsPaused(false)
{
}

GameBase::~GameBase()
{
	;
}

void GameBase::run()
{
	mIsRunning = true;
	while(mIsRunning)
	{
		preframe();
		frame();
		postframe();
	}
}

void GameBase::pause()
{
	mIsPaused = true;
	mClock.pause();
}

void GameBase::unpause()
{
	mIsPaused = false;
	mClock.unpause();
}

void GameBase::stop()
{
	mIsRunning = false;
}

const GameClock& GameBase::getClock() const
{
	return mClock;
}

GameClock& GameBase::getClock()
{
	return mClock;
}