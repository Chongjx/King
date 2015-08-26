#include "Dialogs.h"


Dialogs::Dialogs(void)
{
	ID=0;
	Text="";
}


Dialogs::~Dialogs(void)
{
}

void Dialogs::InitDialogs(int ID, string Text)
{
	this->ID = ID;
	this->Text = Text;
}

int Dialogs::GetID(void)
{
	return ID;
}

string Dialogs::GetText(void)
{
	return Text;
}
