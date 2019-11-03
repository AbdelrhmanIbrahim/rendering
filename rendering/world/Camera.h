#pragma once

#include "math/Vector.h"
#include "math/Matrix.h"
#include "math/Gfx.h"

namespace world
{
	struct Camera
	{
		//view
		math::vec3f pos, target, up;

		//projection
		float left, right, top, bottom;
		float near;
		float far;
	};

	inline Camera
	camera_new()
	{
		Camera self{};
		self.pos = math::vec3f{0, 0, 5};
		self.target = math::vec3f{0, 0, 0};
		self.up = math::vec3f{0, 1, 0};
		self.left = -1;
		self.right = 1;
		self.bottom = -1;
		self.top = 1;
		self.near = 0.1;
		self.far = 1000;

		return self;
	}

	inline void
	camera_viewport(Camera& self, const math::vec2f& viewport)
	{
		self.left = -viewport[0] / 2;
		self.right = viewport[0] + self.left;
		self.bottom = -viewport[1] / 2;
		self.top = viewport[1] + self.bottom;
	}

	inline void
	camera_view_config(Camera& self, const math::vec3f& eye, const math::vec3f& target, const math::vec3f& up)
	{
		self.pos = eye;
		self.target = target;
		self.up = up;
	}

	inline math::Mat4f
	camera_view_matrix(const Camera& self)
	{
		auto fwd = math::normalize(self.target - self.pos);
		auto right = math::normalize(math::cross(fwd, self.up));
		auto up = math::cross(right, fwd);

		math::Mat4f view{};
		view[0] = math::vec4f{ right[0], up[0], -fwd[0], 0 };
		view[1] = math::vec4f{ right[1], up[1], -fwd[1], 0 };
		view[2] = math::vec4f{ right[2], up[2], -fwd[2], 0 };
		view[3] = math::vec4f{ -dot(self.pos, right), -dot(self.pos, self.up), dot(self.pos, fwd), 1 };

		return view;
	}

	inline math::Mat4f
	camera_proj_matrix(const Camera& self)
	{
		math::Mat4f proj{};
		float distance = self.far - self.near;
		float aspect_ratio = (self.right - self.left) / (self.top - self.bottom);
		proj[0][0] = self.near / self.right;
		proj[1][1] = self.near / self.top;
		proj[2][2] = -(self.near + self.far) / distance;
		proj[2][3] = -1.0f;
		proj[3][2] = -2.0f*self.far*self.near / distance;

		return proj;
	}

	inline math::Mat4f
	camera_view_proj(const Camera& self)
	{
		return camera_proj_matrix(self) * camera_view_matrix(self);
	}

	inline math::vec2f
	camera_viewport(const Camera& self)
	{
		return math::vec2f{self.right-self.left, self.top-self.bottom};
	}
};

