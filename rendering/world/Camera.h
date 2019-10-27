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
		self.near = 1000;
		self.far = -1000;

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

	inline math::Mat4f
	camera_view_matrix(const Camera& self)
	{
		auto pos = self.pos;
		auto forward = math::normalize(self.target - pos);
		auto up = math::cross(math::vec3f{1,0,0}, forward);
		auto right = math::cross(forward, up);

		math::Mat4f view{};
		view[0] = math::vec4f{ right[0], right[1], right[2], 0 };
		view[1] = math::vec4f{ up[0], up[1], up[2], 0 };
		view[2] = math::vec4f{ forward[0], forward[1], forward[2], 0 };
		view[3] = math::vec4f{ -pos[0], -pos[1], -pos[2], 0 };

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

