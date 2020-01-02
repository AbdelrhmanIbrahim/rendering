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
		//self->meshes.push_back(object3d_create(math::Y_AXIS, 0.0f, vec3f{ 10,1,10 }, vec3f{ 0,-1.5,0 }, "../rendering/res/stls/floor.stl"));
		self->meshes.push_back(object3d_create(math::Y_AXIS, 0.0f, vec3f{ 1,1,1 }, vec3f{ -2,1,-20 }, "../rendering/res/stls/sphere.stl"));
		self->meshes.push_back(object3d_create(math::Y_AXIS, 0.0f, vec3f{ 1,1,1 }, vec3f{ 2,1,1 }, "../rendering/res/stls/sphere.stl"));
		self->cam = camera_new();

		return self;
	}

	void
	world_free(World* w)
	{
		for(auto mesh : w->meshes)
			object3d_delete(mesh);
		w->meshes.clear();

		delete w;
	}
}