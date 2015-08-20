#ifndef STATEMACHINE_H
#define STATEMACHINE_H

class StateMachine
{
	
public:

	// An enum of generic states
	enum STATE
	{
		ALERT,
		DEAD,
		IDLE,
		RUN,
		SLEEP,
		WALK,
		MAX_STATE,
	};

	// Initialise this class instance
	StateMachine(void);
	~StateMachine(void);

	// Update the state machine
	void Update(double dt);

	// Sets state with the enum as parameter
	void SetState(STATE NewState);
	// Return the current state
	StateMachine::STATE GetState(void);

private:

	// state enum object
	STATE state;
};



#endif