#pragma once

#include "gl/Uniform.h"

#include <vector>

namespace rndr
{
    struct Postprocessing_Pass
    {
        const char* frag_shader_path;
        std::vector<glgpu::Uniform> cpu_uniforms;
    };
};    