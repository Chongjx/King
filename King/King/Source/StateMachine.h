#ifndef STATEMACHINE_H
#define STATEMACHINE_H

class StateMachine
{
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

public:

	// Initialise this class instance
	StateMachine(void);
	~StateMachine(void);

	// Update the state machine
	void Update(double dt);

	// Sets state with the enum as parameter
	void SetState(StateMachine::STATE);

private:

	// state enum object
	StateMachine::STATE state;
};



#endif