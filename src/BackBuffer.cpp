#include "BackBuffer.h"

// Default Constructor.
BackBuffer::BackBuffer(HWND in_mainWindowHandle, int in_width, int in_height)
{
	// Save a copy of the main window handle before making changes.
	this->mainWindowHandle = in_mainWindowHandle;

	// Get a handle to the device context associated with the main window.
	HDC deviceContextHandle = GetDC(in_mainWindowHandle);

	// Set the Width and Height of the BackBuffer based on the input.
	this->backBufferWidth  = in_width;
	this->backBufferHeight = in_height;

	// Create and assign a compatible device context for the main window.
	this->deviceContext = CreateCompatibleDC(deviceContextHandle);

	// Create and assign a compatible surface bitmap based on the device context.
	this->surfaceBitmap = CreateCompatibleBitmap(deviceContextHandle, in_width, in_height);

	// Release the resources reserved while creating the device context.
	ReleaseDC(in_mainWindowHandle, deviceContextHandle);

	// Assign the surface bitmap to the device context.
	this->previousSurfaceBitmap = (HBITMAP)SelectObject(this->deviceContext, this->surfaceBitmap);

	// Create a brush we can use for drawing.
	HBRUSH brushBlack = (HBRUSH)GetStockObject(BLACK_BRUSH);
	HBRUSH brushWhite = (HBRUSH)GetStockObject(WHITE_BRUSH);
	
	// Assign the brush to the device context.
	HBRUSH brush = (HBRUSH)SelectObject(this->deviceContext, brushBlack);

	// Clear the BackBuffer area by drawing a filled white rectangle.
	Rectangle(this->deviceContext, 0, 0, this->backBufferWidth, this->backBufferHeight);

	// Restore the original brush.
	SelectObject(this->deviceContext, brush);
}

// Default Destructor.
BackBuffer::~BackBuffer()
{
	// Delete and release the Surface Bitmap resource.
	SelectObject(this->deviceContext, this->previousSurfaceBitmap);
	DeleteObject(this->previousSurfaceBitmap);

	// Delete and release the Device Context resource.
	DeleteDC(this->deviceContext);
}

// Returns the Device Context.
HDC BackBuffer::DeviceContext()
{
	return this->deviceContext;
}

// Returns the BackBuffer Height.
int BackBuffer::Height()
{
	return this->backBufferHeight;
}

// Presents the contents of the BackBuffer to the screen.
void BackBuffer::Present()
{
	// Get the device contexthandle  for the main window.
	HDC deviceContextHandle = GetDC(this->mainWindowHandle);

	// Copy the contents of the BackBuffer to the screen / window client area.
	BitBlt(deviceContextHandle, 0, 0, this->backBufferWidth, this->backBufferHeight, this->deviceContext, 0, 0, SRCCOPY);

	// Release the Device Context resource.
	ReleaseDC(this->mainWindowHandle, deviceContextHandle);
}

// Returns the BackBuffer Width.
int BackBuffer::Width()
{
	return this->backBufferWidth;
}
