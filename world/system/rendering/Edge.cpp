#include "world/system/rendering/Edge.h"

#include "world/component/Camera.h"
#include "world/component/Mesh.h"
#include "world/component/Transform.h"

#include "math/Vector.h"

namespace world
{
	namespace system
	{
		void
		edge_run(rndr::Edge edge, ecs::World& w)
		{
			//fetch system req components
			auto cam = ecs::world_active_components<world::Camera>(w)[0];
			auto b_meshes = ecs::world_active_components<world::Mesh>(w);
			auto b_transforms = ecs::world_active_components<world::Transform>(w);

			math::vec2f viewport = world::camera_viewport(cam);
			rndr::edge_set(edge, viewport);

			math::Mat4f vp = camera_view_proj(cam);
			for (int i = 0; i < b_meshes.size; ++i)
				rndr::edge_draw(edge, vp, &b_meshes[i], &b_transforms[i], math::vec4f{ 1,1,1,1 });
		}
	};
};