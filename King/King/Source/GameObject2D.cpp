#include "GameObject2D.h"

GameObject2D::GameObject2D(void) :
	render(false)
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