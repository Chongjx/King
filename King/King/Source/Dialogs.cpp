#include "Dialogs.h"


Dialogs::Dialogs(void)
{
	ID=0;
	Text="";
	textSpeed=0;
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

void Dialogs::InitSetting(int textSpeed, string mesh)
{
	this->textSpeed = textSpeed;
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

int Dialogs::GetTextSpeed(void)
{
	return textSpeed;
}

string Dialogs::getMesh(void)
{
	return mesh;
}