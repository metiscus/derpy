#ifndef GAMECLOCK_H_
#define GAMECLOCK_H_

class GameClock
{
public:
	// Units are milliseconds since the epoch
	typedef unsigned long long Timestamp;
	
	GameClock();

	Timestamp getWallTime() const;
	Timestamp getTime() const;

	void pause();
	void unpause();

	bool isPaused() const;

private:
	bool mIsPaused;
	Timestamp mStartTime;
	Timestamp mPauseTime;
	Timestamp mPauseOffset;
};

#endif // GAMECLOCK_H_