#ifndef GUARDS_H
#define GUARDS_H

#include "AI.h"
#include "Guards_StateMachine.h"

class Guards: public AI
{
	
public:

	// Default constructor and destructor
	Guards(void);
	~Guards(void);

	// Initialise this class instance
	void Init(Vector2 pos, Vector2 dir, SpriteAnimation* sa, int tiles, Room& room, string waypoint);
	// Update the character
	void Update(double dt);

	// Change animation
	void changeAni(Guards_StateMachine::GUARD_STATE unitState);

	void PathFinding(int worldWidth, int worldHeight, int tileSize, double dt);

private:

	// StateMachine object
	Guards_StateMachine guardStateMachine;

};

#endif