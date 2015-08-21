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
	void Init(Vector2 pos, Vector2 dir, SpriteAnimation* sa, int tiles, int mapLocation);
	// Update the character
	void Update(double dt);

	// Get the player's energy
	double GetEnergy(void);
	// Updates the player's energy
	void UpdateEnergy(double dt);

	// To contrain player at certain point of map
	void ConstrainPlayer();

private:
	// Energy data
	double energy;
	const double MAX_ENERGY;
};

#endif
