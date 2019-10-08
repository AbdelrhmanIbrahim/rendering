#include "world/World.h"

using namespace geo;

namespace world
{
	World*
	world_create()
	{
		World* self = new World;

		//triangle for now (revisit)
		static Vertex vs[3] = { { 0.5, -0.5, 0 } , { 0, 0.5, 0 } , { -0.5, -0.5, 0 } };
		static unsigned int is[3] = { 0, 1, 2 };
		self->mesh = mesh_create(vs, is);

		return self;
	}

	void
	world_free(World* w)
	{
		//revisit
		mesh_delete(w->mesh);

		//delete w;
	}
}