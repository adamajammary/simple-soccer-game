#include "Timer.h"

// Default Constructor.
Timer::Timer() {
	// Initialize class properties.
	this->framesPassed = 0;

	// Initialize the main game start timer.
	this->gameStartTime = this->CurrentTime();
}

// Default Destructor.
Timer::~Timer() {
}

// Returns the current time as milliseconds that have passed since 1.1.1970 00:00:00 UTC.
__int64 Timer::CurrentTime()
{
	// Main variables.
	__int64		   currentTime;
	time_t		   currentTime_seconds;
	unsigned short currentTime_milliseconds;

	// Create a Time Buffer.
	struct _timeb currentTimeBuffer;

	// Get the Current Time and assign it to the Time Buffer.
	_ftime_s(&currentTimeBuffer);

	// Extract the seconds from the Timer Buffer.
	currentTime_seconds = currentTimeBuffer.time;
	
	// Extract the remaining partial second in milliseconds from the Timer Buffer.
	currentTime_milliseconds = currentTimeBuffer.millitm;
	
	// Convert seconds to milliseconds, and add the remaining milliseconds to get the total Current Time.
	currentTime = (__int64)((currentTime_seconds * 1000) + currentTime_milliseconds);
	
	// Return the result.
	return currentTime;
}

// Calculates and returns the FPS (Frames Per Second) speed.
double Timer::FramesPerSecond()
{
	// Main variables.
	double	framesPerSecond	= 0.0;
	__int64 totalTimePassed	= 0;
	
	// Get the total time in seconds that has passed since the game was started.
	totalTimePassed = (__int64)(this->TotalTimePassed() / 1000);

	// Avoid division errors by dividing by 0.
	if ((totalTimePassed > 0) && (this->framesPassed > 0.0)) 
	{
		// Calculate the number of frames rendered per second.
		framesPerSecond = (double)(framesPassed / totalTimePassed);
	}
	
	// Return the result.
	return framesPerSecond;	
}

// Increases the frames passed counter, should be called at the end of each frame rendering.
void Timer::IncreaseFrameCounter()
{
	this->framesPassed++;
}

// Pause the timer.
void Timer::Pause()
{
	this->pausedTimeStart = this->CurrentTime();

	this->Paused = true;
}

// Initializes all timer values.
void Timer::Start() 
{
	// Start time counters.
	this->framesPassed    = 0;
	this->gameStartTime   = this->CurrentTime();
	this->pausedTime	  = 0;
	this->pausedTimeStart = 0;
}

// Resets all timer values.
void Timer::Stop()
{
	// Reset time counters.
	this->framesPassed    = 0;
	this->gameStartTime   = 0;
	this->pausedTime	  = 0;
	this->pausedTimeStart = 0;
}

// Returns the total time that has passed since the game was started.
__int64 Timer::TotalTimePassed()
{
	// Main variables.
	__int64 totalTimePassed   = 0;
	__int64 currentPausedTime = 0;

	// Calculate the total time in milliseconds that has passed since the game was started.
	totalTimePassed = (__int64)(this->CurrentTime() - this->gameStartTime);

	// Calculate the paused time currently not completed.
	currentPausedTime = (__int64)(this->CurrentTime() - this->pausedTimeStart);
	
	// If the game is still paused we need to subtract the current time while the game is still paused.
	if (this->Paused)
	{
		totalTimePassed = (__int64)((totalTimePassed - this->pausedTime) - currentPausedTime);
	}
	else
	{
		// Otherwise if the game has been unpaused we can just subtract the total paused time.
		totalTimePassed = (__int64)(totalTimePassed - this->pausedTime);
	}

	// Return the result.
	return totalTimePassed;
}

// Unpause the timer.
void Timer::Unpause()
{
	this->pausedTime += (__int64)(this->CurrentTime() - this->pausedTimeStart);

	this->Paused = false;
}
