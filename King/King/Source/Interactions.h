#ifndef INTERACTIONS_H
#define INTERACTIONS_H

#include <vector>
#include "GameObject2D.h"
#include "Item.h"
#include "Prisoners.h"
#include "Guards.h"

using std::string;

class Interactions
{
public:
	enum InteractionStatus
	{
		NotInteracting,
		Interacting,
		Interacted,
		MaxStatus,
	};

	Interactions(void);
	~Interactions(void);

	void setInteractionStatus(InteractionStatus);
	void setName(string name);

	int getInteractionStatus(void);
	string getName(void);

private:

	string name;
	InteractionStatus currentStatus;
};

#endif