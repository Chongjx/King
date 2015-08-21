#ifndef AI_H
#define AI_H

#include "Character.h"

class AI: public Character
{
public:



	// Default constructor and destructor
	AI(void);
	~AI(void);
	
	// Initialise this class instance
	void Init(void);
	// Update the character
	void Update(double dt, bool updating);

	// Set destination for AI
	void SetDestination(float x, float y);
	// Get destination for AI
	Vector2 GetDestination(void);

	void changeAni(StateMachine::STATE unitState);

private:

	Vector2 destination;

};

#endif