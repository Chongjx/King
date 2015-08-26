#ifndef DIALOGS_H
#define DIALOGS_H
#include <string>
#include"Vector2.h"
using std::string;
#pragma once
class Dialogs
{
public:
	Dialogs(void);
	~Dialogs(void);

	void InitDialogs(int ID, string text);

	int GetID(void);
	string GetText(void);

private:
	int ID;
	string Text;
};
#endif

