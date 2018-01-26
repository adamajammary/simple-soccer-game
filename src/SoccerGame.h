#include "Circle.h"
#include "Sprite.h"
//#include <windows.h>
#include "RECTANGLE.h"

#ifndef SOCCER_GAME_H
#define SOCCER_GAME_H

class SoccerGame
{
	// Public properties and methods.
	public:
		// Default Constructor.
		SoccerGame(HINSTANCE in_mainApplicationInstance, HWND in_mainWindowHandle, Vector2D in_mainWindowCenter);
		
		// Default Destructor.
		~SoccerGame();

		// Animates the game by moving objects to their new positions.
		void Animate(float in_changeInTime);
		
		// Pauses the game.
		void Pause();
		
		// Renders the sprite to the BackBuffer.
		void Render(HDC in_backBufferDeviceContext, HDC in_spriteDeviceContext);

		// Unpauses the game.
		void Unpause();

		// Public properties.
		bool Paused;
		int	 PlayerLeftScore;
		int  PlayerRightScore;

	// Private properties and methods.
	private:
		// Animate the Ball by moving it to the new position.
		void AnimateBall(float in_changeInTime);
		
		// Animate the Left Player by moving the player to the new position.
		void AnimateLeftPlayer(float in_changeInTime);
		
		// Animate the Right Player by moving the player to the new position.
		void AnimateRightPlayer(float in_changeInTime);
		
		// Increase the score when a player has scored a goal.
		void IncreaseScore(bool in_leftPlayer);
		
		// Check if a player has kicked the ball.
		bool PlayerKickedTheBall(Sprite* in_player);
		
		// Private properties.
		Sprite*		Ball;
		POINT		CurrentMousePosition;
		HINSTANCE	MainApplicationInstance;
		Vector2D	MainWindowCenter;
		HWND		MainWindowHandle;
		Sprite*		GameBoard;
		RECTANGLE	GameBoardBounds;
		POINT		LastMousePosition;
		Sprite*		PlayerLeft;
		RECTANGLE	PlayerLeftBounds; 
		RECTANGLE	PlayerLeftGoal;
		Vector2D	PlayerLeftStartPosition;
		Sprite*		PlayerRight;
		RECTANGLE	PlayerRightBounds;
		RECTANGLE	PlayerRightGoal;
		float		PlayerRightRecoverTime;
		Vector2D	PlayerRightStartPosition;
		const float SPEED_BALL_MAX;
		const float SPEED_PLAYER_RIGHT;

};

#endif
