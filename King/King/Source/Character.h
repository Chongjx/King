#ifndef CHARACTER_H
#define CHATACTER_H

#include "Vector2.h"
#include "GameObject2D.h"
#include "SpriteAnimation.h"

class Character : public GameObject2D
{

public:

	// Default constructor and destructor
	Character(void);
	~Character(void);

	// Initialise this class instance
	void Init(void);
	// Update the character
	void Update(double dt);

	/*Movement contains both walk and run*/

	// Movement up 
	void MoveUp(double dt);
	// Movement down
	void MoveDown(double dt);
	// Movement left
	void MoveLeft(double dt);
	// Movement right
	void MoveRight(double dt);

	/*************************************/

	// Set field of view of character based on tiles
	void SetFOV(int tiles);
	// Get field of view of character based on tiles
	int GetFOV(void);

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

	// FOV tile range
	int tiles;

	// Inventory inventory

};


#endif