#include "Circle.h"
#include "Vector2D.h"
#include <windows.h>

#ifndef SPRITE_H
#define SPRITE_H

class Sprite
{
	// Public properties and methods.
	public:
		// Default Constructor.
		Sprite(
			HINSTANCE	    in_mainApplicationInstance, 
			int			    in_imageID, 
			int		        in_maskID, 
			const Circle&   in_boundingCircle, 
			const Vector2D& in_position, 
			const Vector2D& in_velocity
		);

		// Default Destructor.
		~Sprite();

		// Moves the Sprite based on the Velocity and the input Change In Time.
		void Animate(float in_changeInTime);
		
		// Returns the Bitmap Image Height.
		int Height();

		// Renders the sprite to the BackBuffer.
		void Render(HDC in_backBufferDeviceContext, HDC in_spriteDeviceContext);

		// Returns the Bitmap Image Width.
		int Width();

		// Public properties.
		Circle   BoundingCircle;
		Vector2D Position; 
		Vector2D Velocity;
		
	protected:
		// Protected properties.
		BITMAP    image;
		HBITMAP   imageHandle;
		HINSTANCE mainApplicationInstance;
		BITMAP    mask;
		HBITMAP   maskHandle;

	private:
		// Make sure creating a new Sprite object based on an existing Sprite object can only be accessed internally/privately.
		Sprite(const Sprite& rhs);

		// Make sure assigning the Sprite object to another Sprite object variable can only be accessed internally/privately.
		Sprite& operator=(const Sprite& rhs);
};

#endif
