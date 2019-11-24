#include "world/World.h"
#include "world/3Dobject.h"
#include "world/Camera.h"

using namespace math;

namespace world
{
	World*
	world_create()
	{
		World* self = new World;
		self->obj = object3d_create(math::Y_AXIS, 0.0f, vec3f{ 1,1,1 }, vec3f{ 0,0,0 }, "../rendering/res/stls/sphere.stl");
		self->cam = camera_new();

		return self;
	}

	void
	world_free(World* w)
	{
		//revisit
		object3d_delete(w->obj);
		delete w;
	}
}