#pragma once

#include "engine/renderer/Pass.h"

#include "math/Vector.h"

namespace rndr
{
	typedef struct IPostprocessor* Postprocessor;

	Postprocessor
	postprocessor_create();

    void
    postprocessor_free(Postprocessor self);

    /*any postprocessing pass must have in its fragment shader
    1) layout (binding = 0) uniform sampler2D in_tex;
    2) in vec2 out_uv;
    to be able to sample from the in texture as the output texture of each pass is being fed to the next pass.*/
    void
    postprocessor_effect_add(Postprocessor self, rndr::Postprocessing_Pass& pass);

	glgpu::Texture
    postprocessor_run(Postprocessor self, glgpu::Texture in, math::Vec2f viewport);
};