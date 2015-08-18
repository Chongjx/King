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
	void Update(void);

	// Movement up 
	void MoveUp(double dt);
	// Movement down
	void MoveDown(double dt);
	// Movement left
	void MoveLeft(double dt);
	// Movement right
	void MoveRight(double dt);

	// Set activeness of a character
	void SetActive(bool active);
	// Get activeness of a character
	bool GetActive(void);

	// Set field of view of character based on tiles
	void SetFOV(int tiles);
	// Get field of view of character based on tiles
	int GetFOV(void);

	//Collision codes to to ensure every character collides with collidable tiles or object
	void Collision(void);

private:

	Vector2 theCharacterPosition;
	bool active;
	int tiles;

};


#endif