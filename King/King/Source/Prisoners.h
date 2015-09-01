#ifndef PRISIONERS_H
#define PRISIONERS_H

#include "AI.h"
#include "Prisoners_StateMachine.h"

class Prisoners: public AI
{
	
public:
	
	// Default constructor and destructor
	Prisoners(void);
	~Prisoners(void);

	// Initialise this class instance
	void Init(Vector2 pos, Vector2 dir, SpriteAnimation* sa, int tiles, Room* room, string waypoint);
	// Update the character
	void Update(int worldWidth, int worldHeight, int tileSize, double dt);

	void Patrolling(int tileSize, double dt);

	void changeAni(Prisoners_StateMachine::PRISONER_STATE unitState);

private:

	//StateMachine object
	Prisoners_StateMachine prisonerStateMachine;
	double randTimer;

};

#endif