#ifndef PRISONERS_STATEMACHINE_H
#define PRISONERS_STATEMACHINE_H

#include "StateMachine.h"

class Prisoners_StateMachine: public StateMachine
{

public:

	enum PRISONER_STATE
	{
		IDLE_STATE,
		WALK_STATE,
		RUN_STATE,
		TALK_STATE,
		SLEEP_STATE,
		////////////
		LEPAK_STATE,
		WALK_OUT_STATE,
		WALK_IN_STATE,

		PRISONER_MAX_STATE,
	};


	// Default constructor and destructor
	Prisoners_StateMachine(void);
	~Prisoners_StateMachine(void);

	// Sets state with the enum as parameter
	void SetState(PRISONER_STATE NewState);
	// Return the current state
	PRISONER_STATE GetState(void) const;


private:

	// state enum object
	PRISONER_STATE state;

};







#endif