#include "Mouse.h"

Mouse::Mouse()
{
}

Mouse::Mouse(string config)
{
	Branch mouseBranch = TextTree::FileToRead(config);

	Config();
}

Mouse::~Mouse()
{
}

void Mouse::Config(void)
{
	mouseBranch.printBranch();

	for (vector<Branch>::iterator branch = mouseBranch.childBranches.begin(); branch != mouseBranch.childBranches.end(); ++branch)
	{
		if (branch->branchName == "SetUp")
		{
			for (vector<Attribute>::iterator attri = branch->attributes.begin(); attri != branch->attributes.end(); ++attri)
			{
				Attribute tempAttri = *attri;
				if (tempAttri.name == "currentPos")
				{
					string xCoord;
					string yCoord;
					int lastContinue = 0;
					for (unsigned j = 0; j < tempAttri.value.size() && j != ','; ++j)
					{
						xCoord += tempAttri.value[j];
						lastContinue = j;
					}

					for (unsigned j = lastContinue; j < tempAttri.value.size(); ++j)
					{
						yCoord += tempAttri.value[j];
					}

					this->currentPosX = stoi(xCoord);
					this->currentPosY = stoi(yCoord);
				}

				else if (tempAttri.name == "sensitivity")
				{
					this->sensitivity = (float)(stoi(tempAttri.value));
				}
			}
		}
	}

	this->diffPos.SetZero();
	this->lastPos.SetZero();
	this->mouseYaw = 0;
	this->mousePitch = 0;

	for (int i = LEFT_BUTTON; i < MAX_BUTTON; ++i)
	{
		mouseButtonStatus[i] = false;
	}
}

void Mouse::Init(string config)
{
	mouseBranch = TextTree::FileToRead(config);

	Config();
}

void Mouse::Update()
{
	for (int i = LEFT_BUTTON; i < MAX_BUTTON; ++i)
	{
		mouseButtonStatus[i] = false;
	}

	glfwGetCursorPos(m_window, &currentPosX, &currentPosY);

	diffPos.x = (float)currentPosX - lastPos.x;
	diffPos.y = (float)currentPosY - lastPos.y;

	mouseYaw = (float) (diffPos.x * 0.0174555555555556f * sensitivity); // 3.142f / 180.0f );
	mousePitch = (float) (diffPos.y * 0.0174555555555556f * sensitivity); // 3.142f / 180.0f );

	lastPos.Set((float)currentPosX, (float)currentPosY);

	for (int i = LEFT_BUTTON; i < MAX_BUTTON; ++i)
	{
		if (glfwGetMouseButton(m_window, i) == GLFW_PRESS)
		{
			mouseButtonStatus[i] = true;
		}
	}
}

void Mouse::setMouseYaw(double value)
{
	this->mouseYaw = value;
}

void Mouse::setMousePitch(double value)
{
	this->mousePitch = value;
}

void Mouse::setSensitivity(float sensitivity)
{
	this->sensitivity = sensitivity;
}

void Mouse::setMousePos(const double xCoord, const double yCoord)
{
	this->currentPosX = xCoord;
	this->currentPosY = xCoord;
}

double Mouse::getCurrentPosX(void)
{
	return this->currentPosX;
}

double Mouse::getCurrentPosY(void)
{
	return this->currentPosY;
}

Vector2 Mouse::getLastPos(void)
{
	return this->lastPos;
}

Vector2 Mouse::getDiffPos(void)
{
	return this->diffPos;
}

double Mouse::getMouseYaw(void) const
{
	return this->mouseYaw;
}

double Mouse::getMousePitch(void) const
{
	return this->mousePitch;
}

float Mouse::getSensitivity(void) const
{
	return this->sensitivity;
}