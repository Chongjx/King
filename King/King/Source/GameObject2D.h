#ifndef GAME_OBJECT_2D_H
#define GAME_OBJECT_2D_H

#include <string>

#include "Mesh.h"

using std::string;

class GameObject2D
{
public:
	GameObject2D(void);
	~GameObject2D(void);

	void setRender(bool render);
	bool getRender(void) const;

	void setMesh(Mesh* mesh);
	Mesh* getMesh(void) const;
protected:
	bool render;
	Mesh* mesh;
};

#endif