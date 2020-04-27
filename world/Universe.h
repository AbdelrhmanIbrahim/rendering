#include "ecs/World.h"

#include "world/system/scripting/Script.h"

namespace world
{
	struct Universe
	{
		ecs::World world;

		//scripts to connect between certain components
		world::system::Script_System script_sys;
	};

	Universe
	universe_new();

	void
	universe_free(Universe& u);

	bool
	universe_load_stl(Universe& u, const char* path);

	void
	universe_init_scene(Universe& u);

	void
	universe_scripts_run(Universe& u);
};