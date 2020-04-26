#include "world/system/rendering/Skybox.h"

#include "world/component/Camera.h"

#include "math/Vector.h"

namespace world
{
	namespace system
	{
		Skybox_System
		skybox_hdr_new(const char* hdr_path)
		{
			Skybox_System self{};
			self.skybox = rndr::skybox_renderer_hdr_create(hdr_path);
			return self;
		}

		void
		skybox_run(Skybox_System sys, ecs::World& w)
		{
			//get bags
			auto cam = ecs::world_active_components<world::Camera>(w)[0];
			rndr::skybox_renderer_draw(sys.skybox, &cam);
		}

		void
		skybox_free(Skybox_System sys)
		{
			rndr::skybox_renderer_free(sys.skybox);
		}
	};
};