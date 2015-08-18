#include "Buttons.h"

Buttons::Buttons(void)	:
	buttonName(""),
	rotation(0.f),
	status(BUTTON_RELEASED)
{
	pos.SetZero();
	scale.SetZero();
}

Buttons::~Buttons(void)
{
}

void Buttons::Init(void)
{
}

void Buttons::Update(void)
{
}

void Buttons::Config(void)
{

}

void Buttons::setName(string name)
{
	this->buttonName = name;
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

void Buttons::setStatus(BUTTON_STATUS status)
{
	this->status = status;
}

string Buttons::getName(void) const
{
	return this->buttonName;
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

Buttons::BUTTON_STATUS Buttons::getStatus(void) const
{
	return this->status;
}