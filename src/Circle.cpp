#include "Circle.h"

// Default Constructor.
Circle::Circle()
{
	// Initialize class properties.
	this->Radius	  = 0.0f;
	this->CenterPoint = Vector2D(0.0f, 0.0f);
}

// Alternate Constructor that takes one float and one Vector2D for input.
Circle::Circle(float in_radius, const Vector2D& in_centerPoint)
{
	// Initialize class properties.
	this->Radius	  = in_radius;
	this->CenterPoint = in_centerPoint;
}

// Checks if two Circles have collided by checking if their areas intersect.
bool Circle::CollisionDetected(Circle& in_targetCircle, Vector2D& in_normal)
{
	// Get the distance between the center points of the two circles.
	Vector2D distance = (Vector2D)(in_targetCircle.CenterPoint - this->CenterPoint);

	// Check if the areas of the two circles intersect.
	// If the distance between the two center points is equal to the sum of the two radiuses, the circles are tangent/touching.
	// If the distance between the two center points is greater than the sum of the two radiuses, the circles are intersecting/overlapping.
	if (distance.Length() <= this->Radius + in_targetCircle.Radius)
	{
		// Get a normalization of the distance vector.
		in_normal = distance.Normalize();

		// If the circles are intersecting/overlapping, move the Target Circle so that they are tangent/touching.
		in_targetCircle.CenterPoint = this->CenterPoint + (in_normal * (this->Radius + in_targetCircle.Radius));

		return true;
	}

	return false;
}
