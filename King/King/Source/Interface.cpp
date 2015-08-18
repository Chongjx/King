#include "Interface.h"

Interface::Interface(void) :
	interfaceType(-1)
{
	buttons.clear();
}

Interface::~Interface(void)
{
	buttons.clear();
}