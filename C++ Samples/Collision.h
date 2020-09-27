/******************************************************************************/
/*!
@file   Collision.h
@author Rohit Saini
@date   9/11/2020
@brief
	Collider class, and its functions to determine if it is colliding with another collider

*/
/**************************************************************************/

#pragma once

#include "Component.h"
#include "Vector.h"
#include "Matrix.h"

//To store which side the collider hits an object
enum class ColliderSide
{
	LEFT = 0,
	RIGHT,
	TOP,
	BOTTOM,
	INSIDE,
	NONE
};

//To store whether it is a circle collider or rectangle
enum class ColliderType
{
	RECT = 0,
	CIRCLE
};

//to use for the collider boundaries
struct ColliderBox
{

public:
	ColliderBox();
	//create collider box with specific bounds (offset x and y from top left, and then width and height)
	ColliderBox(float x, float y, float w, float h);
	float x, y, w, h;
};

class Collider : public Component
{
public:

	/**
	  @brief
		Constructs an empty collider of type rectangle
	*/
	Collider();

	/**
	  @brief
		Constructs a rectangular collider component with the given height/width, collider will be
		from the top left
	*/
	Collider(float width, float height, ColliderType type = ColliderType::RECT);

	/**
	  @brief
		Constructs a collider component with the given radius
	*/
	Collider(float radius, ColliderType type = ColliderType::CIRCLE);

	/**
	  @brief
		Constructs a collider component with the given boundaries, offsets are from TOP LEFT of the sprite of the object.
		Offsets are from the game object / object's position.
	*/
	Collider(float offsetX, float offsetY, float width, float height, ColliderType type = ColliderType::RECT);

	/**
	 @brief
	Constructs a collider component with the given boundaries
	*/
	Collider(ColliderBox bounds, ColliderType type = ColliderType::RECT);

	/**
	  @brief
		Gets the radius of this collider

	  @return
		The radius of this collider
	*/
	float getRadius() const;

	/**
	  @brief
		Updates the collider

	  @param dt
		Time elapsed since last frame
	*/
	void Update(float dt);

	//update collision boundaries for when the sprite changes
	void UpdateCollisionBoundaries(ColliderBox newBox);

	//if the collider is colliding using circular collision (radius)
	bool collidingWithCircle(Collider* other);

	//if the collider is colliding using rectangular collision (bounds)
	bool collidingWithRect(Collider* other);

	//get whether it is a circular or rectangular collider
	ColliderType getColliderType() const;

	//get which side it collided with (left, right, top, bottom, inside, none)
	ColliderSide getVerticalCollisionSide() const;
	ColliderSide getHorizontalCollisionSide() const;

	//set the collision side of the collider
	void setHorizontalCollisionSide(ColliderSide side);

	void setVerticalCollisionSide(ColliderSide side);

	/**
	  @brief
		Returns whether this collider is colliding with the given collider

	  @param other
		The collider to check for collision with

	  @return
		Whether this collider is colliding with the given collider (true or false)
	*/
	bool collidingWith(Collider* other);

private:

	float radius_; //!< Radius of circle collider

	ColliderBox bounds_; //the x offset, y offset, and size of the collider box

	ColliderSide horizontalCollisionSide_; //which side of the collider collides with a different one horizontally
	ColliderSide verticalCollisionSide_; //which side of the collider collides with a different one vertically

	ColliderType colliderType_; //the type of collider (circle or rectangle)

	bool HorizontalCollision(Vector worldPos, Vector otherWorldPos, Collider* other);

	bool VerticalCollision(Vector worldPos, Vector otherWorldPos, Collider* other);
};
