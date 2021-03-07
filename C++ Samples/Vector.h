/***************************************************************************/
	/*!
	@file   Vector.h
	@author Rohit Saini
	@par    Course: GAM200
	@par    Copyright 2020 Digipen Institute of Technology
	@date   09/03/2020
	@brief
	  contains vector functions
	*/
/**************************************************************************/

#pragma once
#include <string>
#include "Trace.h"

// The vector class
class Vector
{
	public:

		//Creates a vector with specified x and y values
		Vector(float x, float y);

		//Creates a vector from integer types
		Vector(int x, int y);

		//	Constructs a zero vector
		Vector()
		{
			x_ = 0.0f;
			y_ = 0.0f;
		}

		//adds 2 position vectors
		Vector operator+(const Vector& v) const;

		//self-addition for a vector
		Vector& operator+=(const Vector& v);

		//self-addition for a vector by a float
		Vector& operator+=(float f);

		//subtracts 2 position vectorss
		Vector operator-(const Vector& v) const;

		//self-subtraction for a vector
		Vector& operator-=(const Vector& v);

		//self-subtraction for a vector by a float
		Vector& operator-=(float f);

		//scaling a vector by a constant
		Vector operator*(float scale) const;
		
		//dot product of 2 vectors
		float operator*(const Vector& v) const;

		//checks inequality between 2 vectors
		bool operator!=(const Vector& v) const;

		//checks equality between 2 vectors
		bool operator==(const Vector& v) const;

		//returns the length of the vector
		float length() const;

		//modifies the vector values to be normalized
		void normalize();

		//returns the normalized vector values in a new vector
		Vector normalized() const;

		//converts the vector into a printable string format (x, y) to 2 decimal places
		std::string toString() const;

		//sets the vector values to the parameters
		void set(float x, float y);

		//returns the x value of the vector
		float getX() const;

		//returns the y value of the vector
		float getY() const;

		// Sets the new x component to the given one
		void setX(float x);

		// Sets the new y component to the given one
		void setY(float y);

		//render vector (x, y) as slider for i am gui
		void RenderForImGui();

		//returns whether the vector has the value 0,0
		bool isZeroVector();

		//returns the vector x as a specific integral type
		template<typename T>
		T getX() const
		{
			return (T) x_;
		}

		//returns the vector y as a specific integral type
		template<typename T>
		T getY() const
		{
			return (T) y_;
		}

		Vector& Zero() { x_ = 0.0f; y_ = 0.0f; return *this; }

		// Prints a debug message with this vectors values
		void Print(const char *label) const {
			Trace::Log("Vector", "%s: [%.1f, %.1f]", label, x_, y_);
		}

		Vector& operator-() {
			x_ = -x_;
			y_ = -y_;
			return *this;
		}

		Vector operator-() const { return Vector(-x_, -y_); }

	private:

		//the position variables
		float x_, y_;


};