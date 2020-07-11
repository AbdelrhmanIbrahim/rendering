#pragma once

#include "gl/glgpu.h"

#include "math/Vector.h"

namespace rndr
{
	typedef struct IPostprocessor* Postprocessor;

	Postprocessor
	postprocessor_create();

    void
    postprocessor_free(Postprocessor self);

    //TODO: add uniforms as a parameter (revisit)
    void
    postprocessor_effect(Postprocessor self, const char* frag_shader);

	glgpu::Texture
    postprocessor_run(Postprocessor self, math::vec2f view_port);
};