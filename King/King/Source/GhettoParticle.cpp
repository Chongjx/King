#include "GhettoParticle.h"

GhettoParticle::GhettoParticle(PARTICLE_TYPE typeValue)
	: type(typeValue),
	scale(1, 1),
	active(false),
	speed(0)
{
}

GhettoParticle::~GhettoParticle()
{
}