#pragma once

#include "ecs/Entity.h"

namespace infra
{
	namespace manager
	{
		struct Selector
		{
			ecs::Entity selected_entity;
		};

		inline static Selector
		selector_new()
		{
			Selector self;
			self.selected_entity = ecs::INVALID_ENTITY;

			return self;
		}

		inline static void
		selector_select(Selector& self, ecs::Entity e)
		{
			self.selected_entity = e;
		}

		inline static void
		selector_clear(Selector& self)
		{
			self.selected_entity = ecs::INVALID_ENTITY;
		}
	}
}