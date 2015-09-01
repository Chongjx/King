#ifndef GHETTOPARTICLE_H
#define GHETTOPARTICLE_H

#include "Vector2.h"

struct GhettoParticle
{
	enum PARTICLE_TYPE
	{
		GO_NONE = 0,
		PO_DETECTOR,

		PO_MAX,
	};

	GhettoParticle(PARTICLE_TYPE typeValue = PO_DETECTOR);
	~GhettoParticle(void);

	PARTICLE_TYPE type;
	Vector2 pos;
	Vector2 vel;
	Vector2 scale;

	float speed;
	bool active;
};
#endif
