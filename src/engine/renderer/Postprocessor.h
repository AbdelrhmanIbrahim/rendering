#pragma once

#include "gl/glgpu.h"

#include "math/Vector.h"

#include <vector>

namespace rndr
{
	typedef struct IPostprocessor* Postprocessor;

    struct Pass
    {
        const char* frag_shader_path;
        std::vector<glgpu::Uniform> uniforms;
    };
    
	Postprocessor
	postprocessor_create();

    void
    postprocessor_free(Postprocessor self);

    void
    postprocessor_effect_add(Postprocessor self, Pass& pass);

	glgpu::Texture
    postprocessor_run(Postprocessor self, math::vec2f view_port);
};