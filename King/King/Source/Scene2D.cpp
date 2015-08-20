#include "Scene2D.h"

#include "GL\glew.h"
#include "shader.hpp"
#include "Utility.h"
#include "LoadTGA.h"

// default scene constructor
Scene2D::Scene2D(void)
{
	m_parameters.resize(0);
	lights.resize(0);
	bLightEnabled = true;
}

Scene2D::~Scene2D(void)
{
}

// default scene init
void Scene2D::Init(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// default scene update
void Scene2D::Update(double dt)
{
	fps = (float)(1.f / dt);
}

// dafault scene render
void Scene2D::Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	projectionStack.LoadMatrix(perspective);

	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
}

// default scene clean up
void Scene2D::Exit(void)
{
	glDeleteProgram(m_programID);
}