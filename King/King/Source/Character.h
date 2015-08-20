#ifndef CHARACTER_H
#define CHATACTER_H

#include "Vector2.h"
#include "GameObject2D.h"
#include "SpriteAnimation.h"
#include "StateMachine.h"

class Character : public GameObject2D
{

public:
	// Default constructor and destructor
	Character(void);
	~Character(void);

	// Initialise this class instance
	void Init(void);
	// Update the character
	virtual void Update(double dt);

	/*Movement contains both walk and run*/

	//// Movement up 
	//void walkUp(double dt);
	//// Movement down
	//void walkDown(double dt);
	//// Movement left
	//void walkLeft(double dt);
	//// Movement right
	//void walkRight(double dt);

	void runUp(double dt);
	void runDown(double dt);
	void runLeft(double dt);
	void runRight(double dt);

	/*************************************/

	// Set field of view of character based on tiles
	void SetFOV(int tiles);
	// Get field of view of character based on tiles
	int GetFOV(void);

	//Get character's current position
	void setPos(Vector2 pos);
	void setDir(Vector2 dir);
	void setVel(Vector2 vel);
	void setSize(Vector2 size);
	void setTargetPos(Vector2 targetPos);
	void setState(StateMachine::STATE unitState);
	void setSprite(SpriteAnimation *sa);
	void setAnimation(StateMachine::STATE currentAni);

	Vector2 getPos(void) const;
	Vector2 getDir(void) const;
	Vector2 getVel(void) const;
	Vector2 getSize(void) const;
	Vector2 getTargetPos(void) const;

	bool moveTo(Vector2 destination);

	/*void changeAni(STATE unitState);*/
	// Inventory
	
	// Interactions
	

protected:

	// Movement data
	const double MAX_WALK_SPEED;
	const double MAX_RUN_SPEED;
	double speed;

	//character attributes
	Vector2 pos;
	Vector2 dir;
	Vector2 vel;

	Vector2 size;
	Vector2 targetPos;

	SpriteAnimation *sprite;

	// FOV tile range
	int tiles;

	//StateMachine object
	StateMachine state;

	// Inventory inventory

};


#endif