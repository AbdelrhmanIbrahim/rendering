#include "world/system/rendering/Background.h"
#include "world/component/Camera.h"

#include "math/Vector.h"
#include "math/Matrix.h"

#include "world/component/Vertex.h"

#include "defs/Defs.h"

using namespace math;

namespace world
{
	namespace system
	{
		void
		bg_col_sys_run(rndr::CQuad self, ecs::World& w)
		{
			//fetch system req components
			Camera cam = ecs::world_active_components<world::Camera>(w)[0];
			cquad_set(self, mat4_id(), world::camera_viewport(cam));
			cquad_draw(self, cbg_quad);
		}

		void
		bg_img_sys_run(rndr::TQuad self, ecs::World& w)
		{
			//fetch system req components
			Camera cam = ecs::world_active_components<world::Camera>(w)[0];
			tquad_set(self, mat4_id(), world::camera_viewport(cam));
			tquad_draw(self, tbg_quad);
		}
	};
};