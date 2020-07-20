#include "world/system/rendering/Postprocessor.h"
#include "world/component/Camera.h"

#include "engine/renderer/Postprocessor.h"

#include "math/Vector.h"

namespace world
{
	namespace system
	{
		glgpu::Texture
		postprocess_sys_run(rndr::Postprocessor pp, ecs::World& w, glgpu::Texture in)
		{
			//fetch system req components
			auto cam = ecs::world_active_components<world::Camera>(w)[0];
			auto out = rndr::postprocessor_run(pp, in, glgpu::texture2d_size(in));

			//test only
			{
				io::Image im = io::image_new(3, glgpu::texture2d_size(in));
				glgpu::texture2d_unpack(out, im, EXTERNAL_TEXTURE_FORMAT::RGB, DATA_TYPE::UBYTE);
				io::image_write(im, "F:/Abdo/rendering/src/after_pp.jpg", IMAGE_FORMAT::JPG);
				io::image_free(im);
			}

			//out is in but postprocessed
			return out;
		}
	};
};