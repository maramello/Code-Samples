/******************************************************************************/
/*!
@file   Collision.cpp
@author Rohit Saini
@date   9/11/2020
@brief
  collision class, and its functions to determine if it is colliding with another

*/
/**************************************************************************/


#include "Collision.h"  // Collider class
#include "GameObject.h" // Parent

/// Below is a stub function header

/************************************************************************/
	/**
	  @brief


	  @param param_name
		...

	  @return
		...
	*/
/************************************************************************/

//construct a collider box with the specified bounds
ColliderBox::ColliderBox(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

//construct an empty collider box
ColliderBox::ColliderBox()
{
	x = 0;
	y = 0;
	w = 0;
	h = 0;
}

	/**
	  @brief
		Constructs an empty collider of type rectangle
	*/
Collider::Collider() : Component(Type::Collider)
{
	radius_ = 0;

	bounds_.x = 0;
	bounds_.y = 0;
	bounds_.w = 0;
	bounds_.h = 0;

	horizontalCollisionSide_ = ColliderSide::NONE; //no collision side yet
	verticalCollisionSide_ = ColliderSide::NONE;
	colliderType_ = ColliderType::RECT;	//collider type
}

/**
 @brief
Constructs a collider component with the given boundaries
*/
Collider::Collider(ColliderBox bounds, ColliderType type) : Component(Type::Collider)
{
	radius_ = 0;
	bounds_ = bounds;
	horizontalCollisionSide_ = ColliderSide::NONE; //no collision side yet
	verticalCollisionSide_ = ColliderSide::NONE;
	colliderType_ = type;
}

	/**
	  @brief
		Constructs a collider component with the given radius
	*/
Collider::Collider(float radius, ColliderType type) : Component(Type::Collider)
{
	radius_ = radius;

	bounds_.x = 0;
	bounds_.y = 0;
	bounds_.w = 0;
	bounds_.h = 0;

	horizontalCollisionSide_ = ColliderSide::NONE; //no collision side yet
	verticalCollisionSide_ = ColliderSide::NONE;
	colliderType_ = ColliderType::CIRCLE;	//collider type
};

/**
  @brief
	Constructs a collider component with the given height/width, collider will be
	from the top left
*/
Collider::Collider(float width, float height, ColliderType type) : Component(Type::Collider)
{
	radius_ = 0;

	bounds_.x = 0;
	bounds_.y = 0;
	bounds_.w = width;
	bounds_.h = height;

	horizontalCollisionSide_ = ColliderSide::NONE; //no collision side yet
	verticalCollisionSide_ = ColliderSide::NONE;
	colliderType_ = type;	//collider type
};

	/**
	  @brief
		Constructs a collider component with the given boundaries, offsets are from TOP LEFT of the sprite of the object
	*/
Collider::Collider(float offsetX, float offsetY, float width, float height, ColliderType type) : Component(Type::Collider)
{
	radius_ = 0;

	bounds_.x = offsetX;
	bounds_.y = offsetY;
	bounds_.w = width;
	bounds_.h = height;

	horizontalCollisionSide_ = ColliderSide::NONE; //no collision side yet
	verticalCollisionSide_ = ColliderSide::NONE;
	colliderType_ = type;	//collider type
};

//needed, but can be left blank
void Collider::Update(float dt)
{

}


// Gets the radius of this collider
float Collider::getRadius() const
{
	return radius_;
}

//if the collider is colliding using circular collision (radius)
bool Collider::collidingWithCircle(Collider* other)
{
	// Get position of this object
	Vector position = Parent()->Get<Transform>()->getTranslation();

	// Get position of other object
	Vector otherPosition = other->Parent()->Get<Transform>()->getTranslation();

	// Get the connecting vector
	Vector connectingVector = otherPosition - position;

	// Check if they are colliding
	if ((radius_ + other->getRadius()) > connectingVector.length())
	{
		//set collision side
		other->setHorizontalCollisionSide(ColliderSide::INSIDE);
		other->setVerticalCollisionSide(ColliderSide::INSIDE);
		return true;
	}

	//set collision side
	other->setHorizontalCollisionSide(ColliderSide::NONE);
	other->setVerticalCollisionSide(ColliderSide::NONE);

	return false;
}

//get whether it is a circular or rectangular collider
ColliderType Collider::getColliderType() const
{
	return colliderType_;
}

//get which side it collided with (left, right, none)
ColliderSide Collider::getHorizontalCollisionSide() const
{
	return horizontalCollisionSide_;
}

//get which side it collided with (top, bottom, none)
ColliderSide Collider::getVerticalCollisionSide() const
{
	return verticalCollisionSide_;
}

//set the collision side of the collider
void Collider::setHorizontalCollisionSide(ColliderSide side)
{
	horizontalCollisionSide_ = side;
}

//set the vertical collision side of the collider
void Collider::setVerticalCollisionSide(ColliderSide side)
{
	verticalCollisionSide_ = side;
}

//helper function to determine horizontal collision
bool Collider::HorizontalCollision(Vector worldPos, Vector otherWorldPos, Collider* other)
{
	//on the left side of the other object
	if (worldPos.getX() < otherWorldPos.getX())
	{
		//collision from the right occurred
		if (worldPos.getX() + bounds_.w > otherWorldPos.getX())
		{
			setHorizontalCollisionSide(ColliderSide::RIGHT);
			other->setHorizontalCollisionSide(ColliderSide::LEFT);
			return true;
		}

		//means no collision
		else
		{
			setHorizontalCollisionSide(ColliderSide::NONE);
			other->setHorizontalCollisionSide(ColliderSide::NONE);
			return false;
		}
	}

	//on right side of other object
	else
	{
		//collision from left side
		if (worldPos.getX() < otherWorldPos.getX() +  other->bounds_.x)
		{
			setHorizontalCollisionSide(ColliderSide::LEFT);
			other->setHorizontalCollisionSide(ColliderSide::RIGHT);
			return true;
		}

		//no collision, too far
		else
		{
			setHorizontalCollisionSide(ColliderSide::NONE);
			other->setHorizontalCollisionSide(ColliderSide::NONE);
			return false;
		}
	}

	return false;
}

bool Collider::VerticalCollision(Vector worldPos, Vector otherWorldPos, Collider* other)
{
	//on top of the other object
	if (worldPos.getY() < otherWorldPos.getY())
	{
		//collision from the bottom occurred, other object is below
		if (worldPos.getY() + bounds_.h >= otherWorldPos.getY())
		{
			setVerticalCollisionSide(ColliderSide::BOTTOM);
			other->setVerticalCollisionSide(ColliderSide::TOP);
			return true;
		}

		//means no collision
		else
		{
			setVerticalCollisionSide(ColliderSide::NONE);
			other->setVerticalCollisionSide(ColliderSide::NONE);
			return false;
		}
	}

	//below other object
	else
	{
		//collision from the top side (other object is above)
		if (worldPos.getY() <= otherWorldPos.getY() + other->bounds_.h)
		{
			setVerticalCollisionSide(ColliderSide::TOP);
			other->setVerticalCollisionSide(ColliderSide::BOTTOM);
			return true;
		}

		//no collision, too far
		else
		{
			setVerticalCollisionSide(ColliderSide::NONE);
			other->setVerticalCollisionSide(ColliderSide::NONE);
			return false;
		}
	}

	return false;
}

//if the collider is colliding using rectangular collision (bounds)
bool Collider::collidingWithRect(Collider* other)
{
	Transform* transform = Parent()->Get<Transform>();
	Transform* otherTransform = other->Parent()->Get<Transform>();

	//calculate position with collision box offset
	Vector offset(bounds_.x, bounds_.y);
	Vector worldPos = transform->getTranslation() + offset;

	//calculate other position with collision box offset
	Vector otherOffset(other->bounds_.x, other->bounds_.y);
	Vector otherWorldPos =  otherTransform->getTranslation() + otherOffset;

	//Check for horizontal collision
	if (HorizontalCollision(worldPos, otherWorldPos, other))
	{
		//Check for vertical collision
		if (VerticalCollision(worldPos, otherWorldPos, other))
		{
			return true;
		}
	}

	return false;
}

//update collision boundaries for when the sprite changes
void Collider::UpdateCollisionBoundaries(ColliderBox newBox)
{
	bounds_ = newBox;
}

// Checks whether this collider is colliding with another one
bool Collider::collidingWith(Collider* other)
{
	//make sure the correct collision type is called
	if (colliderType_ == ColliderType::CIRCLE && other->getColliderType() == ColliderType::CIRCLE)
		return collidingWithCircle(other);

	else
		return collidingWithRect(other);
}
