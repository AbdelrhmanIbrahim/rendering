#include "ecs/World.h"

#include "engine/Engine.h"

#include "world/system/scripting/Script.h"
#include "world/system/retrieving/Pick.h"

#include "io/Input.h"

namespace world
{
	struct Universe
	{
		ecs::World world;
		world::system::Pick_System pick_sys;
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
	universe_input_act(Universe& u, math::vec2f win_size, io::Input& i, rndr::Engine engine);

	void
	universe_scripts_run(Universe& u);
};