#include "GameObject2D.h"

GameObject2D::GameObject2D(void) :
	render(false),
	mesh(NULL)
{
}

GameObject2D::~GameObject2D(void)
{
}

void GameObject2D::setRender(bool render)
{
	this->render = render;
}

bool GameObject2D::getRender(void) const
{
	return this->render;
}

void GameObject2D::setMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

Mesh* GameObject2D::getMesh(void) const
{
	return this->mesh;
}