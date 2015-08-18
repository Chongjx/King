#ifndef BUTTONS_H
#define BUTTONS_H

#include "GameObject2D.h"
#include "Vector2.h"
#include "Mesh.h"

using std::string;

class Buttons : public GameObject2D
{
public:
	enum BUTTON_STATUS
	{
		BUTTON_PRESSED,
		BUTTON_RELEASED,
		BUTTON_HOVER,
		MAX_STATUS,
	};

	Buttons(void);
	~Buttons(void);

	void Init(void);
	void Update(void);

	void Config(void);

	void setName(string name);
	void setPos(Vector2 pos);
	void setScale(Vector2 scale);
	void setRotation(float rotation);
	void setStatus(BUTTON_STATUS status);

	string getName(void) const;
	Vector2 getPos(void) const;
	Vector2 getScale(void) const;
	float getRotation(void) const;
	BUTTON_STATUS getStatus(void) const;

private:
	string buttonName;
	Vector2 pos;
	Vector2 scale;
	float rotation;
	BUTTON_STATUS status;
};

#endif