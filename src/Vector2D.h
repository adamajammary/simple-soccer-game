#include <windows.h>

#ifndef VECTOR2D_H
#define VECTOR2D_H

class Vector2D
{
	// Public properties and methods.
	public:
		// Default Constructor.
		Vector2D();

		// Alternate Constructor that takes two floats for input.
		Vector2D(float in_x, float in_y);
		
		// Alternate Constructor that takes an array with two floats for input.
		Vector2D(float in_coordinates[2]);
		
		// Alternate Constructor that takes a POINT object for input.
		Vector2D(const POINT& in_coordinates);

		// Override default - operator.
		Vector2D operator-();
		
		// Override default - operator.
		Vector2D operator-(const Vector2D& in_rhs)const;

		// Override default + operator.
		Vector2D operator+(const Vector2D& in_rhs)const;

		// Override default POINT operator.
		operator POINT();

		// Override default += operator.
		void operator+=(const Vector2D& in_rhs);

		// Override default -= operator.
		void operator-=(const Vector2D& in_rhs);

		// Override default *= operator.
		void operator*=(float in_scalar);

		// Override default /= operator.
		void operator/=(float in_scalar);

		// Returns the Dot Product of two Vectors.
		float DotProduct(const Vector2D& in_rhs);

		// Returns the Vector Length.
		float Length();

		// Normalizes the Vector.
		Vector2D& Normalize();

		// Unknown functionality.
		Vector2D& Reflect(const Vector2D& in_normal);

		// Public properties.
		float x;
		float y;
};

// Define with left hand and right hand sides reversed so we can write both x*y and y*x.
Vector2D operator*(const Vector2D& in_x, float in_y);
Vector2D operator*(float in_y, const Vector2D& in_x);
Vector2D operator/(const Vector2D& in_x, float in_y);
Vector2D operator/(float in_y, const Vector2D& in_x);

#endif
