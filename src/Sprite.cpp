#include <cassert>
#include "Sprite.h"

// Default Constructor.
Sprite::Sprite(
	HINSTANCE	    in_mainApplicationInstance, 
	int			    in_imageID, 
	int		        in_maskID, 
	const Circle&   in_boundingCircle, 
	const Vector2D& in_position, 
	const Vector2D& in_velocity
) {
	// Save a copy of the main window application instance.
	this->mainApplicationInstance = in_mainApplicationInstance;

	// Load the Bitmap resources.
	this->imageHandle = LoadBitmap(in_mainApplicationInstance, MAKEINTRESOURCE(in_imageID));
	this->maskHandle  = LoadBitmap(in_mainApplicationInstance, MAKEINTRESOURCE(in_maskID));

	// Get the Bitmap structure for the Bitmap Image and Bitmap Mask.
	GetObject(this->imageHandle, sizeof(BITMAP), &this->image);
	GetObject(this->maskHandle,  sizeof(BITMAP), &this->mask);

	// Bitmap Image and Mask should be the same dimensions.
	assert(this->image.bmWidth  == this->mask.bmWidth);
	assert(this->image.bmHeight == this->mask.bmHeight);

	// Set the Sprite Bounding Circle based on the input.
	this->BoundingCircle = in_boundingCircle;

	// Set the Sprite Position based on the input.
	this->Position = in_position;

	// Set the Sprite Velocity based on the input.
	this->Velocity = in_velocity;
}

// Default Destructor.
Sprite::~Sprite()
{
	// Delete and release the Bitmap Image Handle resource.
	DeleteObject(this->imageHandle);

	// Delete and release the Bitmap Mask Handle resource.
	DeleteObject(this->maskHandle);
}

// Moves the Sprite based on the Velocity and the input Change In Time.
void Sprite::Animate(float in_changeInTime)
{
	// Update the Sprite's position.
	this->Position += this->Velocity * in_changeInTime;

	// Update bounding circle, too.  That is, the bounding circle moves with the sprite.
	this->BoundingCircle.CenterPoint = this->Position;
}

// Returns the Bitmap Image Height.
int Sprite::Height()
{
	return this->image.bmHeight;
}

// Renders the sprite to the BackBuffer.
void Sprite::Render(HDC in_backBufferDeviceContext, HDC in_spriteDeviceContext)
{
	// Get the Width and Height of the Sprite.
	int width  = this->Width();
	int height = this->Height();

	// Calculate the Top-Left position of the Sprite.
	int positionX = (int)this->Position.x - (width / 2);
	int positionY = (int)this->Position.y - (height / 2);

	// Select the Mask Bitmap.
	HGDIOBJ bitmapObject = SelectObject(in_spriteDeviceContext, this->maskHandle);

	// Draw the Bitmap Mask to the backbuffer with SRCAND.
	BitBlt(in_backBufferDeviceContext, positionX, positionY, width, height, in_spriteDeviceContext, 0, 0, SRCAND); 

	// Select the Image Bitmap.
	SelectObject(in_spriteDeviceContext, this->imageHandle);

	// Draw the Bitmap Image to the backbuffer with SRCPAINT.
	BitBlt(in_backBufferDeviceContext, positionX, positionY, width, height, in_spriteDeviceContext, 0, 0, SRCPAINT);

	// Restore the original bitmap object.
	SelectObject(in_spriteDeviceContext, bitmapObject);
}

// Returns the Bitmap Image Width.
int Sprite::Width()
{
	return this->image.bmWidth;
}
