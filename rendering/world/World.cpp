#include "world/World.h"

using namespace geo;

namespace world
{
	World*
	world_create()
	{
		World* self = new World;
		self->mesh = mesh_create("../rendering/stls/cube.stl");
		return self;
	}

	void
	world_free(World* w)
	{
		//revisit
		mesh_delete(w->mesh);

		delete w;
	}
}