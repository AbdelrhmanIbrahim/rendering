#pragma once
#include "world/3Dobject.h"
#include "world/Camera.h"

namespace world
{
	struct World
	{
		std::vector<object3D> meshes;
		Camera cam;
	};

	World*
	world_create();

	void
	world_free(World* w);
};