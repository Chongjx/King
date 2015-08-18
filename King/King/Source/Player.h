#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"

class Player: public Character
{

public:

	// Default constructor and destructor
	Player(void);
	~Player(void);

	// Initialise this class instance
	void Init(void);
	// Update the character
	void Update(double dt);

	// Set to know which map the player is at now
	void SetMapLocation(int mapLocation);
	// Get to know which map the player is at now
	int GetMapLocation(void);

	// To contrain player at certain point of map
	void ConstrainPlayer(void); /* parameters to be added */

private:

	int mapLocation;

};

#endif
