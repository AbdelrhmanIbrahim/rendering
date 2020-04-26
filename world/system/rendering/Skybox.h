#pragma once

#include "ecs/World.h"
#include "engine/renderer/Skybox.h"

namespace world
{
	namespace system
	{
		struct
		Skybox_System
		{
			rndr::Skybox skybox;
		};

		Skybox_System
		skybox_hdr_new(const char* hdr_path);

		void
		skybox_run(Skybox_System sys, ecs::World& w);

		void
		skybox_free(Skybox_System sys);
	};
};