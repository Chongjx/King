#ifndef CHARACTER_H
#define CHATACTER_H

#include "Vector2.h"
#include "GameObject2D.h"


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

	// Movement up 
	void MoveUp(double dt);
	// Movement down
	void MoveDown(double dt);
	// Movement left
	void MoveLeft(double dt);
	// Movement right
	void MoveRight(double dt);

	// Set field of view of character based on tiles
	void SetFOV(int tiles);
	// Get field of view of character based on tiles
	int GetFOV(void);

	// Inventory
	



private:

	Vector2 characterPosition;
	int tiles;
	//Inventory inventory
};


#endif