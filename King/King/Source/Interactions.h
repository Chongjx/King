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

	void Update();

	void setInteractionStatus(InteractionStatus);
	void setName(string name);

	int getInteractionStatus();
	string getName();

private:

	string name;
	InteractionStatus currentStatus;
};

#endif