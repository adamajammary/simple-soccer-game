#include "Vector2D.h"

#ifndef CIRCLE_H
#define CIRCLE_H

class Circle
{
	// Public properties and methods.
	public:
		// Default Constructor.
		Circle();

		// Alternate Constructor that takes one float and one Vector2D for input.
		Circle(float in_radius, const Vector2D& in_centerPoint);

		// Checks if two Circles have collided by checking if their areas intersect.
		bool CollisionDetected(Circle& in_targetCircle, Vector2D& in_normal);

		// Public properties.
		Vector2D CenterPoint;
		float	 Radius;

};

#endif
