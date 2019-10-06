#pragma once

#include "geometry/Mesh.h"

namespace world
{
	struct World
	{
		//currently a single mesh to render in the world (revisit)
		geo::Mesh mesh;
	};

	World
	world_create();

	void
	world_free(World& w);
};