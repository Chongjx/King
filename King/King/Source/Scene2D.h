#ifndef SCENE_2D
#define SCENE_2D

#include "Scene.h"
#include "Mtx44.h"
#include "Camera2.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "Light.h"

class Scene2D : public Scene
{
public:
	Scene2D(void);
	~Scene2D(void);

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
protected:
	unsigned m_vertexArrayID;
	vector<Mesh*> meshList;
	unsigned m_programID;
	vector<unsigned> m_parameters;

	Camera2 camera;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	vector<Light> lights;

	bool bLightEnabled;

	float fps;
};

#endif