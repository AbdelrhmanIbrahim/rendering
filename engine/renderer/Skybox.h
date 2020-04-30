#pragma once

#include "defs/Defs.h"

namespace world
{
	struct Camera;
};

namespace rndr
{
	typedef struct ISkybox* Skybox;

	Skybox
	skybox_renderer_rgba_create(const char** skybox_imgs_paths, IMAGE_FORMAT format);

	Skybox
	skybox_renderer_hdr_create(const char* skybox_hdr_path);

	void
	skybox_renderer_free(Skybox self);

	void
	skybox_renderer_draw(const Skybox self, const world::Camera* cam);
};