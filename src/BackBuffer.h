#include <windows.h>

#ifndef BACKBUFFER_H
#define BACKBUFFER_H

class BackBuffer
{
	// Public properties and methods.
	public:
		// Default Constructor.
		BackBuffer(HWND in_mainWindowHandle, int in_width, int in_height);

		// Default Destructor.
		~BackBuffer();

		// Returns the Device Context.
		HDC DeviceContext();

		// Returns the BackBuffer Height.
		int Height();

		// Presents the contents of the BackBuffer to the screen.
		void Present();

		// Returns the BackBuffer Width.
		int Width();

	// Private properties and methods.
	private:
		// Make sure creating a new BackBuffer object based on an existing Backbuffer object can only be accessed internally/privately.
		BackBuffer(const BackBuffer& rhs);

		// Make sure assigning the BackBuffer object to another Backbuffer object variable can only be accessed internally/privately.
		BackBuffer& operator=(const BackBuffer& rhs);

		// Private properties.
		int     backBufferHeight;
		int     backBufferWidth;
		HDC     deviceContext;
		HWND    mainWindowHandle;
		HBITMAP previousSurfaceBitmap;
		HBITMAP surfaceBitmap;
};

#endif
