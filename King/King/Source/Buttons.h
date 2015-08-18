#ifndef BUTTONS_H
#define BUTTONS_H

#include "GameObject2D.h"
#include "Vector2.h"
#include "Mesh.h"

using std::string;

class Buttons : public GameObject2D
{
public:
	enum BUTTON_TYPE
	{
		TEXT_BUTTON,
		IMAGE_BUTTON,
		MAX_TYPE,
	};

	enum BUTTON_STATUS
	{
		BUTTON_PRESSED,
		BUTTON_RELEASED,
		BUTTON_HOVER,
		MAX_STATUS,
	};

	Buttons(void);
	~Buttons(void);

	void Init(string name, Mesh* mesh, Vector2 pos, Vector2 scale, float rotation, BUTTON_STATUS status);
	void Update(Vector2 mousePos);

	void setName(string name);
	void setText(string text);
	void setPos(Vector2 pos);
	void setScale(Vector2 scale);
	void setRotation(float rotation);
	void setType(BUTTON_TYPE type);
	void setStatus(BUTTON_STATUS status);

	string getName(void) const;
	string getText(void) const;
	Vector2 getPos(void) const;
	Vector2 getScale(void) const;
	float getRotation(void) const;
	BUTTON_TYPE getType(void) const;
	BUTTON_STATUS getStatus(void) const;
private:
	string buttonName;
	string text;
	Vector2 pos;
	Vector2 scale;
	float rotation;
	BUTTON_TYPE type;
	BUTTON_STATUS status;
};

#endif