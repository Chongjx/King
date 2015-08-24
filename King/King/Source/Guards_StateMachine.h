#ifndef GUARDS_STATEMACHINE_H
#define GUARDS_STATEMACHINE_H

#include "StateMachine.h"

class Guards_StateMachine: public StateMachine
{

public:
	
	enum GUARD_STATE
	{
		IDLE_STATE,
		WALK_STATE,
		RUN_STATE,
		TALK_STATE,
		SLEEP_STATE,
		////////////
		CHASE_STATE,
		PATROL_STATE,
		RETURN_STATE,
		GUARD_MAX_STATE,
	};

	// Default constructor and destructor
	Guards_StateMachine(void);
	~Guards_StateMachine(void);

	// Sets state with the enum as parameter
	void SetState(GUARD_STATE NewState);
	// Return the current state
	GUARD_STATE GetState(void) const;

private:

	// state enum object
	GUARD_STATE state;

};







#endif