#pragma once

#include "ecs/Entity.h"
#include "ecs/World.h"

#include "io/Input.h"

namespace world
{
	struct Input_Handle
	{
		void (*act)(ecs::World&, ecs::Entity, const io::Input&);
	};
};