#include "world/system/rendering/Postprocessor.h"
#include "world/component/Camera.h"

#include "engine/renderer/Postprocessor.h"

#include "math/Vector.h"

namespace world
{
	namespace system
	{
		glgpu::Texture
		postprocess_sys_run(rndr::Postprocessor pp, ecs::World& w, const char* frag_shader_path)
		{
			//fetch system req components
			auto cam = ecs::world_active_components<world::Camera>(w)[0];
			rndr::postprocessor_effect(pp, frag_shader_path);
			return rndr::postprocessor_run(pp, camera_viewport(cam));
		}
	};
};