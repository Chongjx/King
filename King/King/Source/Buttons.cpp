#include "Buttons.h"

#include "KingEngine.h"

Buttons::Buttons(void)	:
	name(""),
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

void Buttons::Init(string name, Mesh* mesh, Vector2 pos, Vector2 scale, float rotation, BUTTON_STATUS status)
{
	this->name = name;
	this->mesh = mesh;
	this->pos = pos;
	this->scale = scale;
	this->rotation = rotation;
	this->status = status;
	this->type = IMAGE_BUTTON;
}

void Buttons::Init(string name, string text, Vector2 pos, Vector2 scale, float rotation, BUTTON_STATUS status)
{
	this->name = name;
	this->text = text;
	this->pos = pos;
	this->scale = scale;
	this->rotation = rotation;
	this->status = status;
	this->type = TEXT_BUTTON;
}

void Buttons::Update(Vector2 mousePos)
{
	// update buttons status based on mouse position
	if (mousePos.x < this->pos.x + this->scale.x * 0.5f && mousePos.x > this->pos.x - this->scale.x * 0.5f)
	{
		if (mousePos.y < this->pos.y + this->scale.y * 0.5f && mousePos.y > this->pos.y - this->scale.y * 0.5f)
		{

		}
	}

	else
	{
		//this->status = released;
	}
}

void Buttons::setName(string name)
{
	this->name = name;
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
	return this->name;
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