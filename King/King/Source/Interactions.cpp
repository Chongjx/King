#include "Interactions.h"


Interactions::Interactions(void)
{
}


Interactions::~Interactions(void)
{
}

void Interactions::setInteractionStatus(InteractionStatus Status)
{
	this->currentStatus = Status;
}

void Interactions::setName(string name)
{
	this->name = name;
}

int Interactions::getInteractionStatus()
{
	return currentStatus;
}

string Interactions::getName()
{
	return name;
}