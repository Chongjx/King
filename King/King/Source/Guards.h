#ifndef GUARDS_H
#define GUARDS_H

#include "AI.h"

class Guards: public AI
{
	
public:

	// Default constructor and destructor
	Guards(void);
	~Guards(void);

	// Initialise this class instance
	void Init(void);
	// Update the character
	void Update(double dt);

private:

};

#endif