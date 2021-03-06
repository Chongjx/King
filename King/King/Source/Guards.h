#ifndef GUARDS_H
#define GUARDS_H

#include "AI.h"
#include "Guards_StateMachine.h"
#include "Player.h"

class Guards: public AI
{
	
public:

	// Default constructor and destructor
	Guards(void);
	~Guards(void);

	// Initialise this class instance
	void Init(Vector2 pos, Vector2 dir, SpriteAnimation* sa, int tiles, Room* room, string waypoint);
	// Update the character
	void Update(int worldWidth, int worldHeight, int tileSize, double dt);

	// Change animation
	void changeAni(Guards_StateMachine::GUARD_STATE unitState);

	void Patrolling(int worldWidth, int worldHeight, int tileSize, double dt);

	bool tileBasedMovement(int worldWidth, int worldHeight, int tileSize, double dt);

	void Chasing(int worldWidth, int worldHeight, int tileSize, double dt);
	void CheckChase(Vector2 playerPos, int tileSize, double dt);
	bool CheckSight(Vector2 playerPos, int tileSize, double dt);
	bool getChase (void);
	void setChase(bool);

	void setStun(bool stun);
	bool getStun(void);

private:

	// StateMachine object
	Guards_StateMachine guardStateMachine;

	bool chase;
	double checkTimer;
	double stunTimer;

	bool stun;
};

#endif