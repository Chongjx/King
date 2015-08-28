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
	void Update(double dt);

	// Set destination for AI
	void SetDestination(void);
	// Get destination for AI
	Vector2 GetDestination(void);

	void changeAni(StateMachine::STATE unitState);

	// Set update status
	void SetUpdate(bool updating);
	// Get update status
	bool GetUpdate(void);

	// Function for AI path finding
	bool CheckDestination(void);
	virtual void Patrolling(int worldWidth, int worldHeight, int tileSize, double dt);

protected:

	Vector2 destination;
	bool updating;

	//WayPoint ID " A, B, C "
	string waypoint;

};

#endif