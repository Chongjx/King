#ifndef INTERFACE_H
#define INTERFACE_H

#include <vector>
#include "Buttons.h"

using std::vector;

class Interface
{
public:
	Interface(void);
	~Interface(void);

	int interfaceType;
	vector<Buttons> buttons;
};

#endif