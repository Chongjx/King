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

	// Set to know which map the player is at now
	void SetMapLocation(int mapLocation);
	// Get to know which map the player is at now
	int GetMapLocation(void);

	// Get the player's energy
	double GetEnergy(void);
	// Updates the player's energy
	void UpdateEnergy(double dt);
	// Replenishing player's energy
	void Recovering();
	// get state
	bool GetRecovering(void);

	// To contrain player at certain point of map
	void ConstrainPlayer();

private:

	// variable to know which map the player is at
	int mapLocation;

	// Energy data
	double energy;
	bool recovering;
	const double MAX_ENERGY, ENERGY_TO_RUN, DEGENERATE_RUN, REGENERATE_WALK, REGENERATE_IDLE; 

};

#endif
