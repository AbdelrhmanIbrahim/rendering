#pragma once

#include "world/3Dobject.h"

namespace world
{
	struct World
	{
		//revisit
		object3D obj;
	};

	World*
	world_create();

	void
	world_free(World* w);
};