#include "ecs/World.h"
#include "ecs/Entity.h"

#include "world/system/scripting/Script.h"

#include "io/Input.h"

namespace world
{
	//any updating data sys will run (or exposed if needed) here
	struct Universe
	{
		ecs::World world;
		world::system::Script_System script_sys;
	};

	Universe
	universe_new();

	void
	universe_free(Universe& u);

	//entities creation
	bool
	universe_3dobject_add(Universe& u, const char* path);

	//systems
	void
	universe_update_sys_run(Universe& u, math::Vec2f win_size, const io::Input& i);

	void
	universe_movement_sys_run(Universe& u, const io::Input& i, ecs::Entity e);

	void
	universe_scripts_sys_run(Universe& u);
};