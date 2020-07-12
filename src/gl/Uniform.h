#pragma once

#include "math/Vector.h"

#include "gl/glgpu.h"

#include "defs/Defs.h"

namespace glgpu
{
	struct Uniform
    {
		unsigned int slot;
		UNIFORM_TYPE type;
		union
		{
			float f;
			math::Vec3f vec3;
			math::Vec4f vec4;
			math::Mat4f mat4;
			glgpu::Texture texture;
		} value;
    };
};