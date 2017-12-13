#pragma once

#ifndef _SDL_H
#include <SDL.h>
#endif // !_SDL_H




class LTimer
{
public:
	//Initializes variables
	LTimer();
	~LTimer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	Uint32 getTicks();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();

private:
	//The colok time when the timer started
	Uint32 mStartTicks;

	//The ticks stored when the timer was paused
	Uint32 mPausedTicks;

	//The timer status
	bool mPaused;
	bool mStarted;
};

inline LTimer::LTimer()
{
	//Initialize the variables
	mStartTicks = 0;
	mPausedTicks = 0;

	mPaused = false;
	mStarted = false;
}

inline LTimer::~LTimer()
{
}

inline void LTimer::start()
{
	//Start the timer
	mStarted = true;

	//Unpause the timer
	mPaused = false;

	//Get the current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

inline void LTimer::stop()
{
	//Stop the timer
	mStarted = false;

	//Unpause the timer
	mPaused = false;

	//Clear tick variable
	mStartTicks = 0;
	mPausedTicks = 0;
}

inline void LTimer::pause()
{
	//If the timer is runing and isn't already paused
	if (mStarted && !mPaused)
	{
		//Pause the timer
		mPaused = true;

		//Calculate the paused ticks
		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}

inline void LTimer::unpause()
{
	//If the timer is runing and paused
	if ( mStarted && mPaused )
	{
		//Unpaused the timer
		mPaused = false;

		//Reset the starting ticks
		mStartTicks = SDL_GetTicks() - mPausedTicks;
		
		//Reset the paused ticks
		mPausedTicks = 0;
	}
}

inline Uint32 LTimer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

	//If the timer is runing
	if (mStarted)
	{
		//If the timer is paused
		if (mPaused)
		{
			//Return the number of ticks when the timer was paused
			time = mPausedTicks;
		}
		else
		{
			//Return the current time minus the state time
			time = SDL_GetTicks() - mStartTicks;
		}
	}
	return time;
}

inline bool LTimer::isStarted()
{
	//Iimer is running and paused or unpaused 
	return mStarted;
}

inline bool LTimer::isPaused()
{
	//Timer is runing and paused
	return mPaused;
}

