#ifndef STATEMACHINE_H
#define STATEMACHINE_H

class StateMachine
{
public:

	// An enum of generic states
	enum STATE
	{
		IDLE_STATE,
		WALK_STATE,
		RUN_STATE,
		TALK_STATE,
		SLEEP_STATE,
		MAX_STATE,
	};

	// Initialise this class instance
	StateMachine(void);
	~StateMachine(void);

	// Sets state with the enum as parameter
	void SetState(STATE NewState);
	// Return the current state
	STATE GetState(void) const;

private:
	// state enum object
	STATE state;
};


#endif