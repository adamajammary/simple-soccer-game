#include <cstdio>
#include "../resource.h"
#include "SoccerGame.h"

// Default Constructor.
SoccerGame::SoccerGame(
	HINSTANCE in_mainApplicationInstance, 
	HWND in_mainWindowHandle, 
	Vector2D in_mainWindowCenter
) : SPEED_BALL_MAX(1000.0f), SPEED_PLAYER_RIGHT(300.0f)
{
	// Save a copy of the main window application instance.
	this->MainApplicationInstance = in_mainApplicationInstance;

	// Save a copy of the main window handle.
	this->MainWindowHandle = in_mainWindowHandle;

	// Save a copy of the main window center point.
	this->MainWindowCenter = in_mainWindowCenter;

	// Initialize the score for both players to 0.
	this->PlayerLeftScore  = 0;
	this->PlayerRightScore = 0;

	// Initially pause the play and let the user start the game when ready.
	this->Paused = true;

	// Initialize the recovery time of the right player to 0.
	this->PlayerRightRecoverTime = 0.0f;

	// Initialize a Bounding Circle to use for the game sprites.
	Circle boundingCircle;

	// Initialize the Velocity of the game sprites to 0.
	Vector2D velocity(0.0f, 0.0f);

	// Set the Center Point of the Game Board.
	Vector2D centerPoint = in_mainWindowCenter;
	
	// Create the Game Board.
	this->GameBoard = new Sprite(this->MainApplicationInstance, IDB_GAMEBOARD, IDB_GAMEBOARD_MASK, boundingCircle, centerPoint, velocity);

	// Set the Center Point and Radius of the Ball.
	boundingCircle.CenterPoint = centerPoint;
	boundingCircle.Radius	   = 25.0f;

	// Create the Ball.
	this->Ball = new Sprite(this->MainApplicationInstance, IDB_BALL, IDB_BALL_MASK, boundingCircle, centerPoint, velocity);
	
	// Set the default location of the Right Player.
	this->PlayerRightStartPosition = Vector2D(850, 300);
	
	centerPoint.x = this->PlayerRightStartPosition.x;
	centerPoint.y = this->PlayerRightStartPosition.y;
 
	// Set the Center Point and Radius of the Right Player.
	boundingCircle.CenterPoint = centerPoint;
	boundingCircle.Radius	   = 40.0f;
	
	// Create the Right Player.
	this->PlayerRight = new Sprite(this->MainApplicationInstance, IDB_PLAYERRIGHT, IDB_PLAYERRIGHT_MASK, boundingCircle, centerPoint, velocity);

	// Set the default location of the Left Player.
	this->PlayerLeftStartPosition = Vector2D(150, 300);
	
	centerPoint.x = this->PlayerLeftStartPosition.x;
	centerPoint.y = this->PlayerLeftStartPosition.y;

	// Set the Center Point and Radius of the Left Player.
	boundingCircle.CenterPoint = centerPoint;
	boundingCircle.Radius	   = 40.0f;

	// Create the Left Player.
	this->PlayerLeft = new Sprite(this->MainApplicationInstance, IDB_PLAYERLEFT, IDB_PLAYERLEFT_MASK, boundingCircle, centerPoint, velocity);

	// Define some sprite coordinates.
	const float GAMEBOARD_LEFTMARGIN       = 45;
	const float GAMEBOARD_TOPMARGIN        = 15;
	float		GAMEBOARD_HEIGHT	       = ((MainWindowCenter.y * 2) - GAMEBOARD_TOPMARGIN);
	float		GAMEBOARD_WIDTH		       = ((MainWindowCenter.x * 2) - GAMEBOARD_LEFTMARGIN);
	const float GAMEBOARD_GOALBOTTOM_X	   = 95;
	const float GAMEBOARD_GOALTOP_Y		   = 225;
	const float GAMEBOARD_GOALBOTTOM_Y	   = 375;
	float		GAMEBOARD_RIGHTGOALSTART_X = ((MainWindowCenter.x * 2) - GAMEBOARD_GOALBOTTOM_X);
	float		PLAYERBOUNDS_LEFTMARGIN    = (GAMEBOARD_LEFTMARGIN * 2);
	float		PLAYERBOUNDS_RIGHTMARGIN   = (GAMEBOARD_WIDTH - GAMEBOARD_LEFTMARGIN);

	// Set the Rectangle Borders/Bounds of the various Sprites in the game.
	this->GameBoardBounds   = RECTANGLE(GAMEBOARD_LEFTMARGIN,		GAMEBOARD_TOPMARGIN, GAMEBOARD_WIDTH,		   GAMEBOARD_HEIGHT);
	this->PlayerLeftBounds  = RECTANGLE(PLAYERBOUNDS_LEFTMARGIN,	GAMEBOARD_TOPMARGIN, MainWindowCenter.x,	   GAMEBOARD_HEIGHT); 
	this->PlayerLeftGoal    = RECTANGLE(GAMEBOARD_LEFTMARGIN,		GAMEBOARD_GOALTOP_Y, GAMEBOARD_GOALBOTTOM_X,   GAMEBOARD_GOALBOTTOM_Y);
	this->PlayerRightBounds = RECTANGLE(MainWindowCenter.x,			GAMEBOARD_TOPMARGIN, PLAYERBOUNDS_RIGHTMARGIN, GAMEBOARD_HEIGHT);
	this->PlayerRightGoal   = RECTANGLE(GAMEBOARD_RIGHTGOALSTART_X, GAMEBOARD_GOALTOP_Y, GAMEBOARD_WIDTH,		   GAMEBOARD_GOALBOTTOM_Y);
}

// Default Destructor.
SoccerGame::~SoccerGame()
{
	// Delete and release the Game Board resource.
	delete this->GameBoard;

	// Delete and release the Left Player resource.
	delete this->PlayerLeft;

	// Delete and release the Right Player resource.
	delete this->PlayerRight;

	// Delete and release the Ball resource.
	delete this->Ball;
}

// Animates the game by moving objects to their new positions.
void SoccerGame::Animate(float in_changeInTime)
{
	// Make sure the game is not paused.
	if (!this->Paused)
	{
		// Animate the game objects/sprites.
		AnimateLeftPlayer(in_changeInTime);
		AnimateRightPlayer(in_changeInTime);
		AnimateBall(in_changeInTime);

		// Decrease the Right Player's Recovery Time as time passes.
		if (this->PlayerRightRecoverTime > 0.0f)
		{
			this->PlayerRightRecoverTime -= in_changeInTime;
		}
	}
}

// Animate the Ball by moving it to the new position.
void SoccerGame::AnimateBall(float in_changeInTime)
{
	// Check if the left player has kicked the ball.
	this->PlayerKickedTheBall(this->PlayerLeft);

	// If right player hits the ball then make a small 10th of a second
	// delay before the red paddle can move away as sort of a
	// "recovery period" after the hit.  This is to model the
	// fact that when a human player hits something, it takes
	// a short period of time to recover from the collision.  
	if (this->PlayerKickedTheBall(this->PlayerRight))
	{
		this->PlayerRightRecoverTime = 0.1f;
	}

	// Set a maximum velocity for the ball.
	if (this->Ball->Velocity.Length() >= SPEED_BALL_MAX)
	{
		this->Ball->Velocity.Normalize() *= SPEED_BALL_MAX;
	}

	// Get the bounding circle of the ball.
	Circle ballBoudningCircle = this->Ball->BoundingCircle;
	
	// If the ball hit the left side of the game board, reflect according to the velocity on X.
	if (ballBoudningCircle.CenterPoint.x - ballBoudningCircle.Radius < this->GameBoardBounds.startPoint.x)
	{
		this->Ball->Velocity.x *= -1.0f;
	}

	// If the ball hit the right side of the game board, reflect according to the velocity on X.
	if (ballBoudningCircle.CenterPoint.x + ballBoudningCircle.Radius > this->GameBoardBounds.endPoint.x)
	{
		this->Ball->Velocity.x *= -1.0f;
	}

	// If the ball hit the top side of the game board, reflect according to the velocity on Y.
	if (ballBoudningCircle.CenterPoint.y - ballBoudningCircle.Radius < this->GameBoardBounds.startPoint.y)
	{
		this->Ball->Velocity.y *= -1.0f;
	}

	// If the ball hit the bottom side of the game board, reflect according to the velocity on Y.
	if (ballBoudningCircle.CenterPoint.y + ballBoudningCircle.Radius > this->GameBoardBounds.endPoint.y)
	{
		this->Ball->Velocity.y *= -1.0f;
	}

	// Make sure puck stays inbounds of the gameboard.
	this->GameBoardBounds.ForceCircleInsideRectangle(this->Ball->BoundingCircle);
	
	// Get the center point of the ball.
	this->Ball->Position = this->Ball->BoundingCircle.CenterPoint;

	// Animate the ball.
	this->Ball->Animate(in_changeInTime);

	// If the left player has scored a goal, increase the score.
	if (this->PlayerLeftGoal.PointIsInsideRectangle(this->Ball->Position))
	{
		this->IncreaseScore(false);
	}

	// If the right player has scored a goal, increase the score.
	if (this->PlayerRightGoal.PointIsInsideRectangle(this->Ball->Position))
	{
		this->IncreaseScore(true);
	}
}

// Animate the Left Player by moving the player to the new position.
void SoccerGame::AnimateLeftPlayer(float in_changeInTime)
{
	// Get the current mouse cursor position and save it to our class property for later use.
	GetCursorPos(&this->CurrentMousePosition);

	// Get the change in mouse position.
	int changeInX = this->CurrentMousePosition.x - this->LastMousePosition.x;
	int changeInY = this->CurrentMousePosition.y - this->LastMousePosition.y;

	// Get the change in mouse position as a 2D Vector.
	Vector2D changeInMousePosition((float)changeInX, (float)changeInY);

	// Get the Velocity (the change in position with respect to time).
	this->PlayerLeft->Velocity = changeInMousePosition / in_changeInTime;

	// Animate the player.
	this->PlayerLeft->Animate(in_changeInTime);

	// Make sure the player stays inside his half field.
	this->PlayerLeftBounds.ForceCircleInsideRectangle(this->PlayerLeft->BoundingCircle);
	
	// Get the center position of the player.
	this->PlayerLeft->Position = this->PlayerLeft->BoundingCircle.CenterPoint;

	// Save the position of the player to the class property for later use.
	this->LastMousePosition = this->PlayerLeft->Position;

	// Converts the Window coordinates of the player position to screen coordinates.
	ClientToScreen(this->MainWindowHandle, &this->LastMousePosition);
	
	// Move mouse cursor to the location of the player.
	SetCursorPos(this->LastMousePosition.x, this->LastMousePosition.y);
}

// Animate the Right Player by moving the player to the new position.
void SoccerGame::AnimateRightPlayer(float in_changeInTime)
{
	if (this->PlayerRightRecoverTime <= 0.0f)
	{
		// Check if the ball is on the right players field.
		if (this->PlayerRightBounds.PointIsInsideRectangle(this->Ball->Position))
		{
			// Get the Velocity of the right player.
			Vector2D rightPlayerVelocity = this->Ball->Position - this->PlayerRight->Position;

			// Normalize the Velocity of the right player.
			rightPlayerVelocity.Normalize();
			
			// Scale the Velocity based on the speed of the right player.
			rightPlayerVelocity *= this->SPEED_PLAYER_RIGHT;
			
			// Assign the right player the newly calculated velocity.
			this->PlayerRight->Velocity = rightPlayerVelocity;
		}
		else
		{
			Vector2D rightPlayerVelocity = this->PlayerRightStartPosition - this->PlayerRight->Position;
			
			if (rightPlayerVelocity.Length() > 5.0f)
			{
				// Normalize the Velocity of the right player.
				rightPlayerVelocity.Normalize();
	
				// Scale the Velocity based on the speed of the right player.
				rightPlayerVelocity *= this->SPEED_PLAYER_RIGHT;
				
				// Assign the right player the newly calculated velocity.
				this->PlayerRight->Velocity = rightPlayerVelocity;
			}
			else
			{
				// Reset the velocity of the right player to 0.
				this->PlayerRight->Velocity = Vector2D(0.0f, 0.0f);
			}
		}

		// Animate the player.
		this->PlayerRight->Animate(in_changeInTime);

		// Make sure the player stays inside his half field.
		this->PlayerRightBounds.ForceCircleInsideRectangle(this->PlayerRight->BoundingCircle);

		// Get the center position of the player.
		this->PlayerRight->Position = this->PlayerRight->BoundingCircle.CenterPoint;
	}
}

// Increase the score when a player has scored a goal.
void SoccerGame::IncreaseScore(bool in_leftPlayer)
{
	// If the left player scored, increase the score of the left player.
	if (in_leftPlayer)
	{
		this->PlayerLeftScore++;
	}
	// Otherwise increase the score of the right player.
	else
	{
		this->PlayerRightScore++;
	}

	// Set the center point of the ball's bounding circle.
	this->Ball->BoundingCircle.CenterPoint = Vector2D(MainWindowCenter.x, MainWindowCenter.y);

	// Reset the velocity of the ball to 0.
	this->Ball->Velocity = Vector2D(0.0f, 0.0f);
	
	// Set the position of the ball to the center of the game board.
	this->Ball->Position = this->Ball->BoundingCircle.CenterPoint;
	
	// Reset the velocity of the left player to 0.
	this->PlayerLeft->Velocity = Vector2D(0.0f, 0.0f);
	
	// Set the position of the left player the default start position.
	this->PlayerLeft->Position = this->PlayerLeftStartPosition;
	
	// Reset the velocity of the right player to 0.
	this->PlayerRight->Velocity = Vector2D(0.0f, 0.0f);
	
	// Set the position of the right player the default start position.
	this->PlayerRight->Position = this->PlayerRightStartPosition;
	
	// Pause the game allowing the player to continue when ready.
	this->Pause();
}

// Pauses the game.
void SoccerGame::Pause()
{
	// Pause the game.
	this->Paused = true;

	// Stop capturing the mouse.
	ReleaseCapture();

	// Display the mouse cursor.
	ShowCursor(true); 
}

// Check if a player has kicked the ball.
bool SoccerGame::PlayerKickedTheBall(Sprite* in_player)
{
	// Main variables.
	Vector2D normal;

	if(in_player->BoundingCircle.CollisionDetected(this->Ball->BoundingCircle, normal))
	{
		// Get the center point of the ball.
		this->Ball->Position = this->Ball->BoundingCircle.CenterPoint;

		// Compute the player velocity relative to the velocity of the ball.
		Vector2D playerVelocity = in_player->Velocity - this->Ball->Velocity;

		// Get the component of the relative velocity along the normal.
		float impulseMagnitude = playerVelocity.DotProduct(normal);

		// Check if the objects are getting close to eachother.
		if (impulseMagnitude >= 0.0f)
		{
			// Project the relative velocity onto the normal.
			Vector2D impulse = impulseMagnitude * normal; 

			// Add the impulse to the ball.
			this->Ball->Velocity += 2.0f * impulse;

			return true;
		}
	}

	return false;
}

// Renders the sprite to the BackBuffer.
void SoccerGame::Render(HDC in_backBufferDeviceContext, HDC in_spriteDeviceContext)
{
	// Main variables.
	static wchar_t playerScore[30];

	// Render the game board.
	this->GameBoard->Render(in_backBufferDeviceContext, in_spriteDeviceContext);

	// Render the left player.
	this->PlayerLeft->Render(in_backBufferDeviceContext, in_spriteDeviceContext);

	// Render the right player.
	this->PlayerRight->Render(in_backBufferDeviceContext, in_spriteDeviceContext);

	// Render the ball.
	this->Ball->Render(in_backBufferDeviceContext, in_spriteDeviceContext);
	
	// Set the BackBuffer Background as Transparent.
	SetBkMode(in_backBufferDeviceContext, TRANSPARENT);

	// Get the current text color used by the backbuffer device context.
	COLORREF backBufferDeviceContextTextColor = GetTextColor(in_backBufferDeviceContext);

	// Set the new text color to white.
	SetTextColor(in_backBufferDeviceContext, 0x00FFFFFF);

	// Render the score of the left player to the screen.
	swprintf(playerScore, sizeof(playerScore), L"Left Player Score: %d", this->PlayerLeftScore);
	TextOutW(in_backBufferDeviceContext, 60, 25, playerScore, (int)wcslen(playerScore));

	// Render the score of the right player to the screen.
	swprintf(playerScore, sizeof(playerScore), L"Right Player Score: %d", this->PlayerRightScore);
	TextOutW(in_backBufferDeviceContext, 800, 25, playerScore, (int)wcslen(playerScore));

	// Restore the original text color used by the backbuffer device context.
	SetTextColor(in_backBufferDeviceContext, backBufferDeviceContextTextColor);
}

// Unpauses the game.
void SoccerGame::Unpause()
{
	// Get the position of the left player.
	POINT position = this->PlayerLeft->Position;
	
	// Converts the Window coordinates of the left player position to screen coordinates.
	ClientToScreen(MainWindowHandle, &position);

	// Move mouse cursor to the location of the left player.
	SetCursorPos(position.x, position.y);
	
	// Get the current mouse cursor position and save it to our class property for later use.
	GetCursorPos(&this->LastMousePosition);

	// Unpause the game.
	this->Paused = false;

	// Start capturing the mouse again.
	SetCapture(MainWindowHandle);

	// Hide the mouse cursor.
	ShowCursor(false); 
}
