#pragma once

#include "ecs/World.h"
#include "engine/renderer/Phong.h"

namespace world
{
	namespace system
	{
		struct
		Phong_System
		{
			rndr::Phong phong;
		};

		Phong_System
		phong_new();

		void
		phong_run(Phong_System sys, ecs::World& w);

		void
		phong_free(Phong_System sys);
	};
};