#include "Objective.h"

Objective::Objective(void) :
Title(""),
	Get(false),
	level(0),
	keyItem(""),
	currentState(OBJECTIVE_INPCOMPLETE)
{
}

Objective::~Objective(void)
{
}

void Objective::initObjctives (string Title,bool Get,int level,string keyItem)
{
	this->Title=Title;
	this->Get=Get;
	this->level=level;
	this->keyItem=keyItem;
}

void Objective::setState(OBJECTIVE_STATE state)
{
	this->currentState = state;
}

void Objective::setTitle (string Title)
{
	this->Title = Title;

}
void Objective::setGet (bool Get)
{
	this->Get = Get;
}
void Objective::setlevel(int level)
{
	this->level = level;
}
void Objective::setkeyItem (string keyItem)
{
	this->keyItem = keyItem;
}

Objective::OBJECTIVE_STATE Objective::getObjectiveState(void) const
{
	return this->currentState;
}

string Objective::getTitle (void)const
{
	return this->Title;
}
bool Objective::getGet (void) const
{
	return this->Get;
}
int Objective::getlevel (void)const
{
	return this->level;
}
string Objective::getkeyItem (void) const
{
	return this->keyItem;
}