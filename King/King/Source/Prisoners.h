#ifndef PRISIONERS_H
#define PRISIONERS_H

#include "AI.h"

class Prisoners: public AI
{
	
public:
	
	enum PRISONER_STATE
	{
		LEPAK_STATE = StateMachine::MAX_STATE,
	};

	// Default constructor and destructor
	Prisoners(void);
	~Prisoners(void);

	// Initialise this class instance
	void Init(void);
	// Update the character
	void Update(double dt);

	void changeAni(StateMachine::STATE unitState);

private:

};

#endif