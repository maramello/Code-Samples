/***************************************************************************/
	/*!
	@file   Vector.cpp
	@author Rohit Saini
	@date   09/03/2020
	@brief
	  contains vector functions
	*/
/**************************************************************************/
#include "Vector.h"
#include "Trace.h"
#include <math.h>
#include <iomanip>
#include <sstream>

//Creates a vector with specified x and y values
Vector::Vector(float x, float y)
{
	x_ = x;
	y_ = y;
}

//Creates a vector from integer types
Vector::Vector(int x, int y)
{
	x_ = (float) x;
	y_ = (float) y;
}

/************************************************************************/
	/*!
	  @brief
		Adds this vector to the given vector.

	  @param v
		The vector to add.

	  \return
		A new vector containing the sum of this and the given vector.
	*/
/************************************************************************/
Vector Vector::operator+(const Vector& v) const
{
	return Vector(x_ + v.x_, y_ + v.y_);
}

//self-addition for a vector
Vector& Vector::operator+=(const Vector& v)
{
	x_ += v.x_;
	y_ += v.y_;

	return *this;
}

//self-addition for a vector by a float
Vector& Vector::operator+=(float f)
{
	x_ += f;
	y_ += f;

	return *this;
}

//self-subtraction for a vector
Vector& Vector::operator-=(const Vector& v)
{
	x_ -= v.x_;
	y_ -= v.y_;

	return *this;
}

//self-subtraction for a vector by a float
Vector& Vector::operator-=(float f)
{
	x_ -= f;
	y_ -= f;

	return *this;
}

//subtracts 2 position vectors
Vector Vector::operator-(const Vector& v) const
{
	return Vector(x_ - v.x_, y_ - v.y_);
}

//scaling a vector by a constant
Vector Vector::operator*(float scale) const
{
	Vector v(x_, y_);

	v.x_ *= scale;
	v.y_ *= scale;

	return v;
}

//dot product of 2 vectors
float Vector::operator*(const Vector& v) const
{
	return (x_ * v.x_) + (y_ * v.y_);
}

//returns the length of the vector
float Vector::length() const
{
	return sqrtf((x_ * x_) + (y_ * y_));
}

//modifies the vector values to be normalized
void Vector::normalize()
{
	//store length of vector
	float l = length();

	if (l <= 0)
	{
		Trace::Log("Vector", "Length returned 0, unable to normalize (divide by 0 error)");
		return;
	}

	//assign normalized values
	x_ = x_ / l;
	y_ = y_ / l;
}

//returns the normalized vector values in a new vector
Vector Vector::normalized() const
{
	//create new vector and then normalize it
	Vector vector(x_, y_);

	vector.normalize();

	return vector;
}

//converts the vector into a printable string format (x, y) to 2 decimal places
std::string Vector::toString() const
{
	//return in string format to 2 decimal places
	std::stringstream stream, stream2;

	//calculate 2 decimal places
	stream << std::fixed << std::setprecision(2) << x_;
	stream2 << std::fixed << std::setprecision(2) << y_;

	//return the whole string
	return "(" + stream.str() + ", " + stream2.str() + ")";

}

//sets the vector values to the parameters
void Vector::set(float x, float y)
{
	x_ = x;
	y_ = y;
}

//returns the x value of the vector
float Vector::getX() const
{
	return x_;
}

//returns the y value of the vector
float Vector::getY() const
{
	return y_;
}

//checks inequality between 2 vectors
bool Vector::operator!=(const Vector& v) const
{
	if (x_ != v.x_)
		return true;

	if (y_ != v.y_)
		return true;

	return false;
}

//checks equality between 2 vectors
bool Vector::operator==(const Vector& v) const
{
	if (x_ != v.x_)
		return false;

	if (y_ != v.y_)
		return false;

	return true;
}