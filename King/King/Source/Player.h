#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include <cmath>

class Player: public Character
{
public:
	// Default constructor and destructor
	Player(void);
	~Player(void);

	// Initialise this class instance
	void Init(Vector2 pos, Vector2 dir, SpriteAnimation* sa, int tiles, Room* room);
	// Update the character
	void Update(double dt);

	void SetAttack(bool attack);
	bool GetAttack(void);

	// Get the player's energy
	void setEnergy(double energy);
	double GetEnergy(void);
	// Updates the player's energy
	void UpdateEnergy(double dt);
	// Replenishing player's energy
	void Recovering();
	// get state
	bool GetRecovering(void);

	// To contrain player at certain point of map
	void ConstrainPlayer(double dt);

private:
	// Energy data
	double energy;
	bool recovering, attack;
	const double MAX_ENERGY, ENERGY_TO_RUN, DEGENERATE_RUN, REGENERATE_WALK, REGENERATE_IDLE, DEGENERATE_ATTACK; 

};

#endif
