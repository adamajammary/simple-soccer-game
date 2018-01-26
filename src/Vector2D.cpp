#include <cmath>
#include "Vector2D.h"

// Default Constructor.
Vector2D::Vector2D()
{
	this->x = 0.0f;
	this->y = 0.0f;
}

// Alternate Constructor that takes two floats for input.
Vector2D::Vector2D(float in_x, float in_y)
{
	this->x = in_x;
	this->y = in_y;
}

// Alternate Constructor that takes an array with two floats for input.
Vector2D::Vector2D(float in_coordinates[2])
{
	this->x = in_coordinates[0];
	this->y = in_coordinates[1];
}

// Alternate Constructor that takes a POINT object for input.
Vector2D::Vector2D(const POINT& in_coordinates)
{
	this->x = (float)in_coordinates.x;
	this->y = (float)in_coordinates.y;
}

// Override default - operator.
Vector2D Vector2D::operator-()
{
	Vector2D result;

	result.x = -this->x;
	result.y = -this->y;

	return result;
}

// Override default - operator.
Vector2D Vector2D::operator-(const Vector2D& in_rhs)const
{
	Vector2D result;

	result.x = this->x - in_rhs.x;
	result.y = this->y - in_rhs.y;

	return result;
}

// Override default + operator.
Vector2D Vector2D::operator+(const Vector2D& in_rhs)const
{
	Vector2D result;

	result.x = this->x + in_rhs.x;
	result.y = this->y + in_rhs.y;

	return result;
}

// Override default POINT operator.
Vector2D::operator POINT()
{
	POINT point = { (int)this->x, (int)this->y };

	return point;
}

// Override default += operator.
void Vector2D::operator+=(const Vector2D& in_rhs)
{
	this->x += in_rhs.x;
	this->y += in_rhs.y;
}

// Override default -= operator.
void Vector2D::operator-=(const Vector2D& in_rhs)
{
	this->x -= in_rhs.x;
	this->y -= in_rhs.y;
}

// Override default *= operator.
void Vector2D::operator*=(float in_scalar)
{
	this->x *= in_scalar;
	this->y *= in_scalar;
}

// Override default /= operator.
void Vector2D::operator/=(float in_scalar)
{
	// Assumes in_scalar is not 0.
	if (in_scalar != 0)
	{
		in_scalar /= in_scalar;
		this->y /= in_scalar;
	}
}

// Define with left hand and right hand sides reversed so we can write both x*y and y*x.

Vector2D operator*(const Vector2D& in_x, float in_y)
{
	Vector2D result;

	result.x = in_x.x * in_y;
	result.y = in_x.y * in_y;

	return result;
}

Vector2D operator*(float in_y, const Vector2D& in_x)
{
	Vector2D result;

	result.x = in_x.x * in_y;
	result.y = in_x.y * in_y;

	return result;
}

Vector2D operator/(const Vector2D& in_x, float in_y)
{
	Vector2D result;

	// Assumes in_y is not 0.
	if (in_y != 0)
	{
		result.x = in_x.x / in_y;
		result.y = in_x.y / in_y;
	}

	return result;
}

Vector2D operator/(float in_y, const Vector2D& in_x)
{
	Vector2D result;

	// Assumes in_y is not 0.
	if (in_y != 0)
	{
		result.x = in_x.x / in_y;
		result.y = in_x.y / in_y;
	}

	return result;
}

// Returns the Dot Product of two Vectors.
float Vector2D::DotProduct(const Vector2D& in_rhs)
{
	// Main variables.
	float dotProduct;

	// Calculate the dot product of the vector.
	dotProduct = (this->x * in_rhs.x) + (this->y * in_rhs.y);

	// Return the results.
	return dotProduct;
}

// Returns the Vector Length.
float Vector2D::Length()
{
	// Main variables.
	float length;

	// Calculate the length of the vector.
	length = sqrtf(
		(this->x * this->x) + (this->y * this->y)
	);

	// Return the results.
	return length;
}

// Normalizes the Vector.
Vector2D& Vector2D::Normalize()
{
	// Assumes the Vector Length is not 0.
	if (this->Length() != 0)
	{
		float length = this->Length();

		this->x /= length;
		this->y /= length;
	}

	return *this;
}

// Unknown functionality.
Vector2D& Vector2D::Reflect(const Vector2D& in_normal)
{
	Vector2D I = *this;

	*this = ((2.0f * -I.DotProduct(in_normal)) * in_normal) + I;

	return *this;
}
