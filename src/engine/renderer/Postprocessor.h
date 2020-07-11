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

    //TODO: add uniforms as a parameter -- the uv attribute name in the frag_shader_path shoudl be out_uv -- (revisit)
    void
    postprocessor_effect(Postprocessor self, const char* frag_shader_path);

	glgpu::Texture
    postprocessor_run(Postprocessor self, math::vec2f view_port);
};