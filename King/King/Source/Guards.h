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
		

		GUARD_MAX_STATE,
	};

	// Default constructor and destructor
	Guards(void);
	~Guards(void);

	// Initialise this class instance
	void Init(void);
	// Update the character
	void Update(double dt);

	void changeAni(StateMachine::STATE unitState);

private:

};

#endif