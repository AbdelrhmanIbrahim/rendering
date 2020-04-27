#include "ecs/World.h"

#include "world/system/scripting/Script.h"

#include "io/Input.h"

namespace world
{
	struct Universe
	{
		ecs::World world;

		//world::system::Camera_System cam_sys;
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
	universe_input_act(Universe& u, io::Input& i, int window_width, int window_height);

	void
	universe_scripts_run(Universe& u);
};