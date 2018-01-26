#include <sys/timeb.h>
#include <time.h>
#include <windows.h>

#ifndef TIMER_H
#define TIMER_H

class Timer {
	// Public properties and methods.
	public:
		// Default Constructor.
		Timer();
		
		// Default Destructor.
		~Timer();

		// Returns the current time as milliseconds that have passed since 1.1.1970 00:00:00 UTC.
		__int64 CurrentTime();

		// Calculates and returns the FPS (Frames Per Second) speed.
		double FramesPerSecond();
		
		// Increases the frames passed counter, should be called at the end of each frame rendering.
		void IncreaseFrameCounter();
		
		// Pause the timer.
		void Pause();

		// Initializes all timer values.
		void Start();

		// Stops the timer.
		void Stop();

		// Returns the total time that has passed since the game was started.
		__int64 TotalTimePassed();

		// Unpause the timer.
		void Unpause();

		// Public properties.
		bool Paused;

	// Private properties and methods.
	private:
		// Private properties.
		__int64 framesPassed;
		__int64 gameStartTime;
		__int64 pausedTime;
		__int64 pausedTimeStart;

};

#endif
