#include "RECTANGLE.h"

// Default Constructor.
RECTANGLE::RECTANGLE()
{
}

// Alternate Constructor that takes two Vector2D vectors for input.
RECTANGLE::RECTANGLE(const Vector2D& in_startPoint, const Vector2D& in_endPoint)
{
	this->startPoint = in_startPoint;
	this->endPoint   = in_endPoint;
}

// Alternate Constructor that takes four float positions for input.
RECTANGLE::RECTANGLE(float in_startPointX, float in_startPointY, float in_endPointX, float in_bottomRightY)
{
	this->startPoint = Vector2D(in_startPointX, in_startPointY);
	this->endPoint   = Vector2D(in_endPointX, in_bottomRightY);
}

// Forces the Target Circle to be inside the RECTANGLE.
void RECTANGLE::ForceCircleInsideRectangle(Circle& in_targetCircle)
{
	// Main variables.
	Vector2D centerPoint = Vector2D(0.0, 0.0);
	float radius		 = 0;

	// Get the Center point of the target circle.
	centerPoint = in_targetCircle.CenterPoint;
	
	// Get the radius of the target circle.
	radius = in_targetCircle.Radius;

	// If the circle is outside the left side of the rectangle, set the X coordinate back inside.
	if ((centerPoint.x - radius) < this->startPoint.x)
	{
		centerPoint.x = this->startPoint.x + radius;
	}
	
	// If the circle is outside the right side of the rectangle, set the X coordinate back inside.
	if ((centerPoint.x + radius) > this->endPoint.x)
	{
		centerPoint.x = this->endPoint.x - radius;
	}

	// If the circle is outside the top side of the rectangle, set the Y coordinate back inside.
	if ((centerPoint.y - radius) < this->startPoint.y)
	{
		centerPoint.y = this->startPoint.y + radius;
	}
	
	// If the circle is outside the bottom side of the rectangle, set the Y coordinate back inside.
	if ((centerPoint.y + radius) > this->endPoint.y)
	{
		centerPoint.y = this->endPoint.y - radius;
	}

	// Apply the new coordinates to the target circle to physically move the circle back inside of the rectangle.
	in_targetCircle.CenterPoint = centerPoint;
}

// Checks if the Vector2D Point is inside the RECTANGLE.
bool RECTANGLE::PointIsInsideRectangle(const Vector2D& in_vectorPoint)
{
	// Main variables.
	bool pointIsInsideRectangle;

	// Check all vector coordinates that their are inside the rectangle area coordinates.
	pointIsInsideRectangle = (
		(in_vectorPoint.x >= this->startPoint.x) && (in_vectorPoint.y >= this->startPoint.y) &&
		(in_vectorPoint.x <= this->endPoint.x)   && (in_vectorPoint.y <= this->endPoint.y)
	);

	// Return the results.
	return pointIsInsideRectangle;
}
