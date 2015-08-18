#include "Buttons.h"

Buttons::Buttons(void)	:
	buttonName(""),
	text(""),
	rotation(0.f),
	type(TEXT_BUTTON),
	status(BUTTON_RELEASED)
{
	pos.SetZero();
	scale.SetZero();
}

Buttons::~Buttons(void)
{
}

/*void Buttons::Init(void)
{
}*/

void Buttons::Update(Vector2 mousePos)
{
}

void Buttons::setName(string name)
{
	this->buttonName = name;
}

void Buttons::setText(string text)
{
	this->text = text;
}

void Buttons::setPos(Vector2 pos)
{
	this->pos = pos;
}

void Buttons::setScale(Vector2 scale)
{
	this->scale = scale;
}

void Buttons::setRotation(float rotation)
{
	this->rotation = rotation;
}

void Buttons::setType(BUTTON_TYPE type)
{
	this->type = type;
}

void Buttons::setStatus(BUTTON_STATUS status)
{
	this->status = status;
}

string Buttons::getName(void) const
{
	return this->buttonName;
}

string Buttons::getText(void) const
{
	return this->text;
}

Vector2 Buttons::getPos(void) const
{
	return this->pos;
}

Vector2 Buttons::getScale(void) const
{
	return this->scale;
}

float Buttons::getRotation(void) const
{
	return this->rotation;
}

Buttons::BUTTON_TYPE Buttons::getType(void) const
{
	return this->type;
}

Buttons::BUTTON_STATUS Buttons::getStatus(void) const
{
	return this->status;
}