#include "Buttons.h"

#include "KingEngine.h"

Buttons::Buttons(void)	:
	name(""),
	text(""),
	rotation(0.f),
	type(TEXT_BUTTON),
	status(BUTTON_IDLE)
{
	pos.SetZero();
	scale.SetZero();
}

Buttons::~Buttons(void)
{
}

void Buttons::Init(string name, string text, Mesh* mesh, Vector2 pos, Vector2 scale, float rotation, Color col, BUTTON_TYPE type, BUTTON_STATUS status)
{
	this->name = name;
	this->text = text;
	this->mesh = mesh;
	this->pos = pos;
	this->scale = scale;
	this->rotation = rotation;
	this->textCol = col;
	this->type = type;
	this->status = status;
}

void Buttons::Update(bool pressed, double mouseX, double mouseY)
{
	// update buttons status based on mouse position
	if (mouseX < this->pos.x + this->scale.x && mouseX > this->pos.x)
	{
		if (mouseY < this->pos.y + this->scale.y && mouseY > this->pos.y)
		{
			if (prevStatus == BUTTON_PRESSED)
			{
				this->status = BUTTON_RELEASED;
			}

			else if (pressed)
			{
				this->status = BUTTON_PRESSED;
			}

			else
			{
				this->status = BUTTON_HOVER;
			}
		}

		else
		{
			this->status = BUTTON_IDLE;
		}
	}

	else
	{
		this->status = BUTTON_IDLE;
	}

	prevStatus = status;

	std::cout << status << std::endl;
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

void Buttons::setColor(Color col)
{
	this->textCol = col;
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

Color Buttons::getColor(void) const
{
	return this->textCol;
}

Buttons::BUTTON_TYPE Buttons::getType(void) const
{
	return this->type;
}

Buttons::BUTTON_STATUS Buttons::getStatus(void) const
{
	return this->status;
}