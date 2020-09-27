/******************************************************************************/
/*!
@file   Player.h
@author Rohit Saini
@date   09/08/2020
@brief
	This is the player file containing player function definitions

*/
/**************************************************************************/

#pragma once
#include "GameObject.h"
#include "Vector.h"
#include "Physics.h"
#include "Transform.h"
#include "Sprite.h"
#include "Data.h"
#include "Collision.h"

//stores constants for specific player actions which are performed by pressing keys
enum class PlayerAction
{
	MOVE_LEFT = 0,
	MOVE_RIGHT,
	STOP,			//for stopping velocity in a specific direction
	JUMP,
	DRAG_OBJECT,
	PUSH_OBJECT,
	TIME_MECHANIC,
};

class Player : public GameObject
{

	public:

		//Constructs the player
		Player();

		//set up initial player settings (image,position,etc.)
		void Init();

		//read the data for the player from the player.json file, all values will be read from there
		void SetInitialData();

		//manually sets the player position through transform
		void SetPosition(Vector newPosition);

		//get player position, simply gets transform's translation
		Vector GetPosition();

		//used perform/call the player mechanics like movement, grabbing, etc. parameter is passed from InputManager
		void ActionCheck(PlayerAction action);

		//update the player
		void Update(float dt);

		//returns whether the player is jumping or not
		bool IsJumping();

		//perform jump mechanic
		void Jump();

		//updates player to be in jumping state or not
		void setJumping(bool jumping);

		// Handles collision between the player and other objects
		void handleCollision(GameObject* other, float dt);

	private:

		//just assume that we need this variable cause the player will die at some point
		bool isDead_;

		//if the player is currently in a jumping state
		bool isJumping_;

		//stores whether the player is on the ground / object right now
		bool isGrounded_;

		//how fast the player moves left and right
		float speed_;

		//how high the player jumps
		float gravity_;

};
