#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <vector>
#include "Buttons.h"

using std::vector;

class GameMenu
{
public:
	GameMenu(void);
	~GameMenu(void);

	vector<Buttons> buttons;
};

#endif