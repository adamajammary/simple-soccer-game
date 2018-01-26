#include "BackBuffer.h"
#include <cassert>
#include <iostream>
#include "../resource.h"
#include "SoccerGame.h"
#include <string>
#include "Timer.h"
#include <windows.h>

using namespace std;

// Initialize the Timer Class.
Timer timer;

// Initialize the BackBuffer Class.
BackBuffer* BackBufferContext = 0;

// Initialize the Main Soccer Game Class.
SoccerGame* Soccer = 0;

// Main Window properties.
HINSTANCE    MainApplicationInstance = 0;
LPCWSTR      MainApplicationName	 = L"Soccer Game";
HMENU        MainMenu				 = 0;
LPCWSTR		 MainWindowCaption  	 = MainApplicationName;
LPCWSTR		 MainWindowClassName	 = L"MainWindowClassName";
HWND         MainWindowHandle		 = 0;
const int	 MainWindowHeight		 = 600;
const int	 MainWindowWidth		 = 1000;
const POINT  MainWindowCenter		 = { (MainWindowWidth/2), (MainWindowHeight/2) };
HDC          SpriteDeviceContext	 = 0;

// Timer properties.
__int64		 InitLoopTime			 = timer.CurrentTime();
__int64		 MilliSecondsPassed		 = 0;

// Game Difficulties.
const float	GAME_DIFFICULTY_EASY	 = 0.002f;
const float	GAME_DIFFICULTY_MEDIUM	 = 0.004f;
const float	GAME_DIFFICULTY_HARD	 = 0.006f;
const float	GAME_DIFFICULTY_CHAMPION = 0.008f;
float		GameDifficulty			 = GAME_DIFFICULTY_EASY;

// About Box Dialog.
INT_PTR CALLBACK AboutBoxProcedure(HWND in_dialogBoxHandle, UINT in_message, WPARAM in_wParam, LPARAM in_lParam);

// Main Window Procedure.
LRESULT CALLBACK MainWindowProcedure(HWND in_mainWindowHandle, UINT in_message, WPARAM in_wParam, LPARAM in_lParam);

// Renders the FPS speed and other Time properties to the screen.
void RenderTimeProperties();

// Renders the Trophy when the player wins.
void RenderTrophy();

// Resets the game, and starts over.
void StartNewGame();

// About Box Dialog.
INT_PTR CALLBACK AboutBoxProcedure(HWND in_dialogBoxHandle, UINT in_message, WPARAM in_wParam, LPARAM in_lParam)
{
	if (in_message == WM_INITDIALOG)
	{
		return true;
	}
	else if (in_message == WM_COMMAND)
	{
		// Close the About Box when the user clicks on the OK button.
		if (LOWORD(in_wParam) == IDOK)
		{
			EndDialog(in_dialogBoxHandle, 0);
		}

		return true;
	}

	return false;
}

// Main Window Procedure.
LRESULT CALLBACK MainWindowProcedure(HWND in_mainWindowHandle, UINT in_message, WPARAM in_wParam, LPARAM in_lParam)
{	
	switch (in_message)
	{	
		case WM_CREATE:
			// Create the Soccer Game.
			Soccer = new SoccerGame(MainApplicationInstance, in_mainWindowHandle, MainWindowCenter);

			// Create the Device Context for the resources.
			SpriteDeviceContext = CreateCompatibleDC(0);

			// Create the BackBuffer context.
			BackBufferContext = new BackBuffer(in_mainWindowHandle, MainWindowWidth, MainWindowHeight);

			return 0;
		case WM_COMMAND:
			switch(LOWORD(in_wParam))
			{
				case ID_FILE_EXIT:
					// Close the application when the user chooses to exit/quit.
					DestroyWindow(MainWindowHandle);

					break;
				case ID_FILE_NEWGAME:
					// Reset the game, and start over.
					StartNewGame();

					break;
				case ID_DIFFICULTY_EASY:
					// Make the EASY menu item checked, and uncheck the rest.
					CheckMenuItem(MainMenu, ID_DIFFICULTY_EASY,     MF_CHECKED);
					CheckMenuItem(MainMenu, ID_DIFFICULTY_MEDIUM,   MF_UNCHECKED);
					CheckMenuItem(MainMenu, ID_DIFFICULTY_HARD,     MF_UNCHECKED);
					CheckMenuItem(MainMenu, ID_DIFFICULTY_CHAMPION, MF_UNCHECKED);

					// Set the game difficulty level to EASY.
					GameDifficulty = GAME_DIFFICULTY_EASY;

					break;
				case ID_DIFFICULTY_MEDIUM:
					// Make the MEDIUM menu item checked, and uncheck the rest.
					CheckMenuItem(MainMenu, ID_DIFFICULTY_EASY,     MF_UNCHECKED);
					CheckMenuItem(MainMenu, ID_DIFFICULTY_MEDIUM,   MF_CHECKED);
					CheckMenuItem(MainMenu, ID_DIFFICULTY_HARD,     MF_UNCHECKED);
					CheckMenuItem(MainMenu, ID_DIFFICULTY_CHAMPION, MF_UNCHECKED);

					// Set the game difficulty level to MEDIUM.
					GameDifficulty = GAME_DIFFICULTY_MEDIUM;

					break;
				case ID_DIFFICULTY_HARD:
					// Make the HARD menu item checked, and uncheck the rest.
					CheckMenuItem(MainMenu, ID_DIFFICULTY_EASY,     MF_UNCHECKED);
					CheckMenuItem(MainMenu, ID_DIFFICULTY_MEDIUM,   MF_UNCHECKED);
					CheckMenuItem(MainMenu, ID_DIFFICULTY_HARD,     MF_CHECKED);
					CheckMenuItem(MainMenu, ID_DIFFICULTY_CHAMPION, MF_UNCHECKED);

					// Set the game difficulty level to HARD.
					GameDifficulty = GAME_DIFFICULTY_HARD;

					break;
				case ID_DIFFICULTY_CHAMPION:
					// Make the CHAMPION menu item checked, and uncheck the rest.
					CheckMenuItem(MainMenu, ID_DIFFICULTY_EASY,     MF_UNCHECKED);
					CheckMenuItem(MainMenu, ID_DIFFICULTY_MEDIUM,   MF_UNCHECKED);
					CheckMenuItem(MainMenu, ID_DIFFICULTY_HARD,     MF_UNCHECKED);
					CheckMenuItem(MainMenu, ID_DIFFICULTY_CHAMPION, MF_CHECKED);

					// Set the game difficulty level to CHAMPION.
					GameDifficulty = GAME_DIFFICULTY_CHAMPION;

					break;
				case ID_HELP_ABOUT:
					// Render the About Dialog Box.
					DialogBox(MainApplicationInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), MainWindowHandle, AboutBoxProcedure);
	
					break;
			}

			return 0;
		case WM_LBUTTONDOWN:
			// Unpause the game if the user clicked the left mouse button.
			Soccer->Unpause();
			timer.Unpause();

			return 0;
		case WM_RBUTTONDOWN:
			// Pause the game if the user clicked the right mouse button.
			Soccer->Pause();
			timer.Pause();

			return 0;
		case WM_DESTROY: 	
			// Clean up and release all game resources.
			delete Soccer;
			delete BackBufferContext;
			DeleteDC(SpriteDeviceContext);
			PostQuitMessage(0); 

			return 0;	
	}

	// Forward any other messages we didn't handle to the Default Window Procedure.
	return DefWindowProc(in_mainWindowHandle, in_message, in_wParam, in_lParam);
}

// Renders the FPS speed and other Time properties to the screen.
void RenderTimeProperties()
{
	// Main variables.
	double		   framesPerSecond = 0;
	static wchar_t newMainWindowCaption[256];
	__int64		   totalTimePassed = 0;

	// Get the total time in seconds that has passed since the game started.
	totalTimePassed = (__int64)(timer.TotalTimePassed() / 1000);
	
	// Get the FPS speed.
	framesPerSecond = timer.FramesPerSecond();

	// Build a new string containing the FPS speed and the time that has passed since the game started.
	swprintf(
		newMainWindowCaption, 
		sizeof(newMainWindowCaption), 
		L"%s - Time: %I64d / 45 secs - FPS: %.0f",
		MainApplicationName, 
		totalTimePassed,
		framesPerSecond
	);

	// Update the Main Window Caption (Title Bar).
	SetWindowTextW(MainWindowHandle, newMainWindowCaption);
}

// Renders the Trophy when the player wins.
void RenderTrophy()
{
	// Main variables.
	static wchar_t winningText[255];
	
	// Set the BackBuffer Background as Transparent.
	SetBkMode(BackBufferContext->DeviceContext(), TRANSPARENT);
	
	// Get the current text color used by the backbuffer device context.
	COLORREF backBufferDeviceContextTextColor = GetTextColor(BackBufferContext->DeviceContext());
	
	// Create a bigger font for the game status header.
	HFONT font = CreateFont(
		48,0,0,0,
		FW_BOLD,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, 
		VARIABLE_PITCH,
		TEXT("Arial")
	);
	
	// Apply the new font to the back buffer.
	SelectObject(BackBufferContext->DeviceContext(), font);
	
	// If the game ended in a tie ...
	if (Soccer->PlayerLeftScore == Soccer->PlayerRightScore)
	{
		// Set the new text color to white.
		SetTextColor(BackBufferContext->DeviceContext(), 0x00FFFFFF);

		swprintf(winningText, sizeof(winningText), L"TOO BAD, A TIE ...");

		// Play the WAV sound file.
		//PlaySound(MAKEINTRESOURCE(IDR_AUDIO_TIE), MainApplicationInstance, SND_SYNC | SND_RESOURCE);
		//PlaySound(NULL, MainApplicationInstance, SND_PURGE);
	}
	// If the user won the game ...
	else if (Soccer->PlayerLeftScore > Soccer->PlayerRightScore)
	{
		// Initialize the Velocity of the Trophy to 0.
		Vector2D velocity(0.0f, 0.0f);

		// Set the location and Radius of the Trophy.
		Circle boundingCircle;
		boundingCircle.CenterPoint = MainWindowCenter;
		boundingCircle.Radius	   = 50.0f;
		
		// Create the Trophy.
		Sprite* trophy = new Sprite(MainApplicationInstance, IDB_TROPHY, IDB_TROPHY_MASK, boundingCircle, MainWindowCenter, velocity);

		// Put the trophy in the middle of the screen.
		trophy->Position = MainWindowCenter;

		// Render the Trophy.
		trophy->Render(BackBufferContext->DeviceContext(), SpriteDeviceContext);

		// Set the new text color to blue.
		SetTextColor(BackBufferContext->DeviceContext(), 0x00FF0000);

		swprintf(winningText, sizeof(winningText), L"CONGRATULATIONS !!!");

		// Play the WAV sound file.
		//PlaySound(MAKEINTRESOURCE(IDR_AUDIO_WIN), MainApplicationInstance, SND_SYNC | SND_RESOURCE);
		//PlaySound(NULL, MainApplicationInstance, SND_PURGE);
	}
	// If the user lost the game ... 
	else
	{
		// Set the new text color to red.
		SetTextColor(BackBufferContext->DeviceContext(), 0x000000FF);

		swprintf(winningText, sizeof(winningText), L"SORRY, MAYBE NEXT TIME ...");

		// Play the WAV sound file.
		//PlaySound(MAKEINTRESOURCE(IDR_AUDIO_LOOSE), MainApplicationInstance, SND_SYNC | SND_RESOURCE);
		//PlaySound(NULL, MainApplicationInstance, SND_PURGE);
	}
	
	// Render the text to the center of the screen horizontally and 50 pixels from the top.
	SetTextAlign(BackBufferContext->DeviceContext(), TA_CENTER);
	TextOutW(BackBufferContext->DeviceContext(), MainWindowCenter.x, 50, winningText, (int)wcslen(winningText));
	
	// Restore the original text color used by the backbuffer device context.
	SetTextColor(BackBufferContext->DeviceContext(), backBufferDeviceContextTextColor);
}

// Resets the game, and starts over.
void StartNewGame()
{
	// Main Window properties.
	MainWindowCaption = MainApplicationName;
	
	// Timer properties.
	InitLoopTime	   = timer.CurrentTime();
	MilliSecondsPassed = 0;

	// Timer Class.
	timer.Stop();
	timer.Start();
	timer.Pause();

	// Re-create the game objects.
	delete Soccer;
	Soccer = new SoccerGame(MainApplicationInstance, MainWindowHandle, MainWindowCenter);

	// Re-create the Device Context for the resources.
	DeleteDC(SpriteDeviceContext);
	SpriteDeviceContext = CreateCompatibleDC(0);

	// Re-create the BackBuffer context.
	delete BackBufferContext;
	BackBufferContext = new BackBuffer(MainWindowHandle, MainWindowWidth, MainWindowHeight);
}

// Main Entry-Point for the Application.
int WINAPI WinMain(
	HINSTANCE in_mainApplicationInstance, 
	HINSTANCE in_legacy16bitApplicationInstance, 
	PSTR	  in_commandLineArguments, 
	int		  in_windowDisplayType
) {
	// Set the Main Application Instance.
	MainApplicationInstance = in_mainApplicationInstance;

	// Main Window Class Structure.
	WNDCLASSW windowClass; 
	windowClass.style         = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc   = MainWindowProcedure;
	windowClass.cbClsExtra    = 0;
	windowClass.cbWndExtra    = 0;
	windowClass.hInstance     = MainApplicationInstance;
	windowClass.hIcon         = LoadIcon(0, IDI_APPLICATION);
	windowClass.hCursor       = LoadCursor(0, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	windowClass.lpszMenuName  = 0;
	windowClass.lpszClassName = MainWindowClassName;

	// Register the Window Class Instance.
	RegisterClassW(&windowClass);

	// Create and load the Main Menu.
	MainMenu = LoadMenu(MainApplicationInstance, MAKEINTRESOURCE(IDR_MENU));
	
	// Make the default game level difficulty selection.
	CheckMenuItem(MainMenu, ID_DIFFICULTY_EASY, MF_CHECKED);

	// Create the Main Window and assign it to the Main Window Handle.
	MainWindowHandle = CreateWindowW(
		MainWindowClassName, 
		MainWindowCaption, 
		WS_OVERLAPPED | WS_SYSMENU, 
		200, 
		200, 
		(MainWindowWidth + 7), 
		(MainWindowHeight + 49), 
		0, 
		MainMenu, 
		MainApplicationInstance, 
		0
	);

	// If the Main Window did not successfully create, throw an error.
	if(MainWindowHandle == 0)
	{
		MessageBoxW(MainWindowHandle, L"Window Creation Failed.", L"Error", MB_OK);

		return 0;
	}
	else
	{
		// Otherwise display the newly created window.
		ShowWindow(MainWindowHandle, SW_NORMAL);
		
		// Perform an initial update of the window.
		UpdateWindow(MainWindowHandle);
		
		// Enter the game loop (windows message loop).
		MSG messageLoop;
		
		// Initialize the MSG object.
		ZeroMemory(&messageLoop, sizeof(MSG));

		// Keep checking windows messages until the user quits/exits.
		while (messageLoop.message != WM_QUIT)
		{
			// Process existing windows messages and remove them from the message queue.
			if(PeekMessage(&messageLoop, 0, 0, 0, PM_REMOVE))
			{
				// Handle the windows message.
				TranslateMessage(&messageLoop);
				DispatchMessage(&messageLoop);
			}
			else
			{	
				// Make sure the timer is not running until the game has started.
				if (Soccer->Paused && !timer.Paused)
				{
					timer.Pause();
				}

				// Keep playing the game until 45 seconds have passed.
				if (timer.TotalTimePassed() < 45000)
				{
					if (!Soccer->Paused)
					{
						// Increase the game timer's frame counter for every loop iteration.
						timer.IncreaseFrameCounter();
					}

					// Get the time passed in milliseconds since the message/game loop started.
					MilliSecondsPassed = (__int64)(timer.CurrentTime() - InitLoopTime);

					// Update the game and render the screen.
					Soccer->Animate(GameDifficulty);

					// Render the frame.
					Soccer->Render(BackBufferContext->DeviceContext(), SpriteDeviceContext);

					if (MilliSecondsPassed >= 1000)
					{
						if (!Soccer->Paused)
						{
							InitLoopTime	   = timer.CurrentTime();
							MilliSecondsPassed = 0;

							// Render the FPS speed and other Time properties to the screen.
							RenderTimeProperties();
						}
					}
				}
				else
				{
					// Render the Trophy if the player won.
					RenderTrophy();

					// Stop the game.
					Soccer->Pause();

					// Stop the timer.
					timer.Pause();

					// Stop capturing the mouse.
					ReleaseCapture();

					// Display the mouse cursor.
					ShowCursor(true);
				}

				// Present the BackBuffer to the Front Window.
				BackBufferContext->Present();
			}
		}
		
		return (int)messageLoop.wParam;
	}

	return 0;
}
