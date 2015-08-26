#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include <string>
#include"Vector2.h"
using std::string;
#pragma once
class Instructions
{
public:
	Instructions(void);
	~Instructions(void);

	void InitInstructions (string Header, string Text);

	string GetHeader (void);
		string GetText (void);

private:
	string Header;
	string Text;
};

#endif