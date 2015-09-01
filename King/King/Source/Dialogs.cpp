#include "Dialogs.h"


Dialogs::Dialogs(void)
{
	ID=0;
	Text="";
	mesh="";
}


Dialogs::~Dialogs(void)
{
}

void Dialogs::InitDialogs(int ID, string Text)
{
	this->ID = ID;
	this->Text = Text;
}

void Dialogs::InitSetting(string mesh)
{
	this->mesh = mesh;
}

int Dialogs::GetID(void)
{
	return ID;
}

string Dialogs::GetText(void)
{
	return Text;
}

string Dialogs::getMesh(void)
{
	return mesh;
}