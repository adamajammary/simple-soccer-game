#include "Circle.h"

#ifndef RECTANGLE_H
#define RECTANGLE_H

class RECTANGLE
{
	// Public properties and methods.
	public:
		// Default Constructor.
		RECTANGLE();

		// Alternate Constructor that takes two Vector2D vectors for input.
		RECTANGLE(const Vector2D& in_startPoint, const Vector2D& in_endPoint);
		
		// Alternate Constructor that takes four float positions for input.
		RECTANGLE(float in_startPointX, float in_startPointY, float in_endPointX, float in_bottomRightY);

		// Forces the Target Circle to be inside the RECTANGLE.
		void ForceCircleInsideRectangle(Circle& in_targetCircle);
		
		// Checks if the Vector2D Point is inside the RECTANGLE.
		bool PointIsInsideRectangle(const Vector2D& in_vectorPoint);

		// Public properties.
		Vector2D endPoint;
		Vector2D startPoint;

};

#endif
