#include "Interface.h"

Interface::Interface(void) :
	interfaceType(-1)
{
	buttons.resize(0);
}

Interface::~Interface(void)
{
	buttons.clear();
}