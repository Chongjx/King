#include "StateMachine.h"

StateMachine::StateMachine()
{

}

StateMachine::~StateMachine()
{

}

void StateMachine::SetState(STATE NewState)
{
	state = NewState;
}

StateMachine::STATE StateMachine::GetState(void) const
{
	return state;
}