#include "Guards_StateMachine.h"

Guards_StateMachine::Guards_StateMachine()
{

}

Guards_StateMachine::~Guards_StateMachine()
{

}

void Guards_StateMachine::SetState(GUARD_STATE NewState)
{
	state = NewState;
}

Guards_StateMachine::GUARD_STATE Guards_StateMachine::GetState(void) const
{
	return state;
}