#ifndef CHARACTER_H
#define CHARACTER_H

#include <iomanip>

#include "Vector2.h"
#include "GameObject2D.h"
#include "SpriteAnimation.h"
#include "StateMachine.h"
#include "Inventory.h"
#include "Room.h"

class Character : public GameObject2D
{
public:
	enum SPRITE_ANI
	{
		IDLE_UP,
		IDLE_DOWN,
		IDLE_LEFT,
		IDLE_RIGHT,
		WALK_UP,
		WALK_DOWN,
		WALK_LEFT,
		WALK_RIGHT,
		RUN_UP,
		RUN_DOWN,
		RUN_LEFT,
		RUN_RIGHT,
		MAX_ANI,
	};

	// Default constructor and destructor
	Character(void);
	~Character(void);

	// Initialise this class instance
	virtual void Init(Vector2 pos, Vector2 dir, Vector2 vel, SpriteAnimation* sa);
	virtual void Init(Vector2 pos, Vector2 dir, SpriteAnimation* sa);
	// Update the character
	virtual void Update(double dt);

	/*Movement contains both walk and run*/
	bool moveTo(void);

	// Movement up
	void moveUp(bool walk, double dt);
	// Movement down
	void moveDown(bool walk, double dt);
	// Movement left
	void moveLeft(bool walk, double dt);
	// Movement right
	void moveRight(bool walk, double dt);

	void tileBasedMovement(int worldWidth, int worldHeight, int tileSize, double dt);
	void tileBasedOffset(void);

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

	Vector2 getPos(void) const;
	Vector2 getDir(void) const;
	Vector2 getVel(void) const;
	Vector2 getSize(void) const;
	Vector2 getTargetPos(void) const;
	StateMachine::STATE getState(void) const;
	SpriteAnimation* getSprite(void) const;

	// Inventory
	CInventory getInventory(void) const;
	void setRoom(Room &currentRoom);

	Room getRoom(void);
	virtual void changeAni(StateMachine::STATE unitState);

protected:
	// Movement data
	const double MAX_WALK_SPEED;
	const double MAX_RUN_SPEED;
	float walkSpeed;
	float runSpeed;

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
	StateMachine stateMachine;

	// Inventory inventory
	CInventory inventory;

	vector<Animation*> animationList;
	SPRITE_ANI currentAni;

	Room currentRoom;
	// variable to know which map the player is at
};

#endif