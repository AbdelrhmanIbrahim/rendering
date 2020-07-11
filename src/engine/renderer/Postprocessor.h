#pragma once

#include "gl/glgpu.h"

#include "math/Vector.h"

#include <vector>

namespace rndr
{
	typedef struct IPostprocessor* Postprocessor;

	Postprocessor
	postprocessor_create();

    void
    postprocessor_free(Postprocessor self);

    void
    postprocessor_effect_add(Postprocessor self, glgpu::Postprocessing_Pass& pass);

	glgpu::Texture
    postprocessor_run(Postprocessor self, math::Vec2f view_port);
};