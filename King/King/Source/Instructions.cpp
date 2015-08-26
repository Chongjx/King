#include "Instructions.h"


Instructions::Instructions(void)
{
	Header="";
	Text="";
}


Instructions::~Instructions(void)
{
}

void Instructions::InitInstructions (string Header, string Text)
{
	this->Header = Header;
	this->Text =Text;
}

string Instructions::GetHeader (void)
{
	return Header;
}
string Instructions::GetText (void)
{
	return Text;
}