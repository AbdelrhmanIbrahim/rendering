#pragma once

#include "world/3Dobject.h"
#include "world/Camera.h"

namespace world
{
	struct World
	{
		object3D obj;
		Camera cam;
	};

	World*
	world_create();

	void
	world_free(World* w);
};