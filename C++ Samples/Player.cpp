/******************************************************************************/
/*!
@file   Player.cpp
@author Rohit Saini
@date   09/08/2020
@brief
	This is the player implementation file

*/
/**************************************************************************/

#include "Player.h"		/// The Player class
#include "Sprite.h"
#include "Physics.h"
#include "Transform.h"
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "Trace.h"

#include <string>

#include "Object.h"		  /// For collision handling
#include "InputManager.h" /// For collision handling and input

//Constructs the player
Player::Player() : GameObject("Player")
{
    // Add components
    Add(new Physics());
    Add(new Transform());
    Add(new Sprite());

    //dimensions of the picture for now
    Add(new Collider(147.0f, 287.0f));

    //Set the player's initial settings
    Init();
}

//set up initial player settings (image,position,etc.)
void Player::Init()
{
    //Read in all the player's initial data (position, size, speed, etc.)
    SetInitialData();
}

//read the data for the player from the player.json file, all values will be read from there
void Player::SetInitialData()
{
    //read data from file to assign values from player.json

    //edit player.json to change player values (position, size, etc.)

    PlayerData data = Data::ReadPlayer();

    //Get the player's sprite and set the image and size of the player
    Sprite* sprite = Get<Sprite>();

    sprite->setTexture(Texture(data.imagePath, data.dimensions.getX<int>(), data.dimensions.getY<int>()));

    //Set the initial position of the player
    SetPosition(data.position);

    speed_ = data.speed; //speed of player
    gravity_ = data.gravity; //gravity on the player, can control how high they jump

    isDead_ = false;
    isGrounded_ = true; //player starts on ground
}

//manually sets the player position through transform
void Player::SetPosition(Vector newPosition)
{
	Transform* transform = Get<Transform>();

    transform->setTranslation(newPosition);
}

//get player position, simply gets transform's translation
Vector Player::GetPosition()
{
    Transform* transform = Get<Transform>();

    return transform->getTranslation();
}

//Update the player
void Player::Update(float dt)
{
    //check if alive
    if (isDead_)
    {
        //oh no the player died, reset from last checkpoint here (yet to do)
        return;
    }

    if (isJumping_)
    {
        Jump();
    }
}

static float startingHeight = 0.0f; //private variable to help calculate jumping starting height and ending height

//perform jump mechanic
void Player::Jump()
{
    Physics* physics = Get<Physics>();

    //if the player is on the ground, make it jump
    if (isGrounded_)
    {
        //store initial starting position for now, since there are no collisions yet to determine the ground
        startingHeight = GetPosition().getY();

        //make the player have an upward velocity, but keep whatever x-velocity there was (can jump diagonally)
        physics->SetVelocity(Vector(physics->GetVelocity().getX(), -speed_));

        //set the acceleration to have it gradually move down
        physics->SetAcceleration(Vector(0.0f, gravity_));

        SetPosition(Vector(GetPosition().getX(), GetPosition().getY() - 1.0f));

        isGrounded_ = false; //no longer on the ground
    }

    //ADD HERE: if there is a collision with anything, stop jumping by setting isJumping_ = false;
    //remove this if and replace with collision detection condition
    if (GetPosition().getY() >= startingHeight)
    {
        //means the player has collided with something and can stop jumping
        isGrounded_ = true;
        isJumping_ = false;

        //remove external y-axis forces (gravity and speed in upward direction)
        physics->SetAcceleration(Vector(0.0f, 0.0f));
        physics->SetVelocity(Vector(physics->GetVelocity().getX(), 0.0f));
    }

}

//used to perform/call the player mechanics like movement, grabbing, etc.
void Player::ActionCheck(PlayerAction action)
{
	//get physics component
	Physics* physics = Get<Physics>();
    // get sprite component
    Sprite* sprite = Get<Sprite>();


	if (getCollisionStatus()->inACollision())
	{
		GameObject* collidee = getCollisionStatus()->getCollidee();

		if (strstr(collidee->getName().c_str(), "Object"))
		{
			// If the player is in collision with a heavy box
			if ( ! ((Object*) collidee)->canBePushedOrPulled())
				return;

			// The object the player is colliding with can be pushed or pulled
			else
			{
				if (InputManager::LETTER_PULL.isDown())
				{
					Debug("PULL");

					// Position and velocity of the player
					Vector coords = GetPosition();
					Vector velocity = Get<Physics>()->GetVelocity();

					// Get the position of this object
					Vector boxPosition = collidee->Get<Transform>()->getTranslation();

					// Player located to the left
					if (coords.getX() < boxPosition.getX())
					{
						if (velocity.getX() < 0.0f) // And the player is moving left
						{
							Debug("Player pulling from LEFT");
							// Pull velocity to the left as well
							collidee->Get<Physics>()->SetVelocity(Vector(velocity.getX(), 0.0f));
						}
						else
						{
							collidee->Get<Physics>()->SetVelocity(Vector(0.0f, 0.0f));
							Get<Physics>()->SetVelocity(Vector(0.0f, 0.0f));
						}

					}

					// Player located to the right
					else if (coords.getX() > boxPosition.getX())
					{
						if (velocity.getX() > 0.0f) // And the player is moving right
						{
							// Push velocity to the left as well
							collidee->Get<Physics>()->SetVelocity(Vector(-velocity.getX(), 0.0f));
							Debug("Player pulling from RIGHT");
						}

						else
						{
							collidee->Get<Physics>()->SetVelocity(Vector(0.0f, 0.0f));
							Get<Physics>()->SetVelocity(Vector(0.0f, 0.0f));
						}
					}

				}
			}
		}
	}

    //check which action is being performed and update accordingly
    switch (action)
    {
        //if the player is trying to move left, apply velocity in that direction
       case PlayerAction::MOVE_LEFT:
           physics->SetVelocity(Vector(-speed_, physics->GetVelocity().getY()));
           sprite->toggleFlipHorizontal(true);
           break;

        //if the player is trying to move right, apply velocity in that direction
       case PlayerAction::MOVE_RIGHT:
           physics->SetVelocity(Vector(speed_, physics->GetVelocity().getY()));
           sprite->toggleFlipHorizontal(false);
           break;

           //if the player is trying to move right, apply velocity in that direction
       case PlayerAction::JUMP:
           if (!isJumping_)
               setJumping(true);
           break;

        //player is stopping moving in a specific direction (left or right)
       case PlayerAction::STOP:
           physics->SetVelocity(Vector(0.0f, physics->GetVelocity().getY()));
           break;
    }
}

//returns whether the player is jumping or not
bool Player::IsJumping()
{
    return isJumping_;
}

//updates player to be in jumping state
void Player::setJumping(bool jumping)
{
    isJumping_ = jumping;
}

// Handles collision between the player and other objects
void Player::handleCollision(GameObject* other, float dt)
{
	Physics* physics = Get<Physics>(); // Get the player physics component
    Sprite* sprite = Get<Sprite>();    // gett the player sprite component

	// Collided with some sort of object
	if (strstr(other->getName().c_str(), "Object"))
	{
		Object* object = (Object*) other;

		// If this is a heavy object
		if (!object->canBePushedOrPulled())
		{
			Debug("Collided with BOX");
			// Remove x velocity because the player can't go through an object?
			   // THIS is done in Object.cpp
		}
	}

}
