#include "Prisoners_StateMachine.h"

Prisoners_StateMachine::Prisoners_StateMachine()
{

}

Prisoners_StateMachine::~Prisoners_StateMachine()
{

}

void Prisoners_StateMachine::SetState(PRISONER_STATE NewState)
{
	state = NewState;
}

Prisoners_StateMachine::PRISONER_STATE Prisoners_StateMachine::GetState(void) const
{
	return state;
}