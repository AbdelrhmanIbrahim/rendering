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
			rndr::postprocessor_effect_add(pp, rndr::Pass{ frag_shader_path, {} });
			auto out = rndr::postprocessor_run(pp, camera_viewport(cam));

			//test only
			{
				/*io::Image im = io::image_new(4, camera_viewport(cam));
				glgpu::texture2d_unpack(tex, im, EXTERNAL_TEXTURE_FORMAT::RGBA, DATA_TYPE::UBYTE);
				io::image_write(im, "F:/Abdo/rendering/src/postprocess.png", IMAGE_FORMAT::PNG);*/
			}

			return out;
		}
	};
};