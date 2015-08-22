#ifndef GUARDS_H
#define GUARDS_H

#include "AI.h"

class Guards: public AI
{
	
public:

	enum GUARD_STATE
	{
		CHASE_STATE = StateMachine::MAX_STATE,
		PATROL_STATE,
		RETURN_STATE,


		GUARD_MAX_STATE,
	};

	// Default constructor and destructor
	Guards(void);
	~Guards(void);

	// Initialise this class instance
	void Init(Vector2 pos, Vector2 dir, SpriteAnimation* sa, int tiles, Room& room);
	// Update the character
	void Update(double dt);

	// Sets patrol area


	void changeAni(StateMachine::STATE unitState);

private:

};

#endif