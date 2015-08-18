#ifndef GAME_OBJECT_2D_H
#define GAME_OBJECT_2D_H

class GameObject2D
{
public:
	GameObject2D(void);
	~GameObject2D(void);

	void setRender(bool render);
	bool getRender(void) const;
protected:
	bool render;
};

#endif