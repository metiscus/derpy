#include "gameclock.h"

#ifdef __linux
#define PlatformIsLinux 1
#include <sys/time.h>
#endif

#include <cassert>
#include <cstdlib>

static inline GameClock::Timestamp GetTimestamp();

GameClock::GameClock()
	: mIsPaused(false)
	, mStartTime(GetTimestamp())
	, mPauseTime(0)
	, mPauseOffset(0)
{
	;
}

GameClock::Timestamp GameClock::getWallTime() const
{
	return GetTimestamp() - mStartTime;
}

GameClock::Timestamp GameClock::getTime() const
{
	return GetTimestamp() - mPauseTime - mStartTime;
}

void GameClock::pause()
{
	assert(!mIsPaused);
	if(!mIsPaused)
	{
		mIsPaused = true;
		mPauseTime = GetTimestamp();
	}
}

void GameClock::unpause()
{
	assert(mIsPaused);
	if(mIsPaused)
	{
		mIsPaused = false;
		mPauseOffset += GetTimestamp() - mPauseTime;
	}
}

bool GameClock::isPaused() const
{
	return mIsPaused;
}

#if PlatformIsLinux
static inline GameClock::Timestamp GetTimestamp()
{
	timeval tv;
	gettimeofday(&tv, NULL);

	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
#endif