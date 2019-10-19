#include "app/app.h"

#include "window/window.h"
#include "gpu_gl/glgpu.h"

#include "world/World.h"
#include "engine/Engine.h"

using namespace world;
using namespace rndr;

namespace app
{
	application::application(int argc, char** argv)
	{
		backend::callbacks_init(argc, argv);
		win::window_create(1400, 800, "rendering journey");
		backend::callbacks_set(this);
		glgpu::graphics_init();

		w = world_create();
		e = engine_create();
	}

	application::~application()
	{
		world_free(w);
		engine_free(e);
	}

	void
	application::run()
	{
		backend::callbacks_run();
	}

	void
	application::update()
	{
		//render the top world
		engine_world_draw(e, w, math::vec2f{ 1400, 800 });

		//swap buffer
		backend::callbacks_update();
	}

	void
	application::mouse_handle(int x, int y)
	{
	}

	void
	application::keyboard_handle(unsigned char c, int x, int y)
	{
	}
};