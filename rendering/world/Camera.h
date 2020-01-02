#pragma once

#include "math/Vector.h"
#include "math/Matrix.h"
#include "math/Gfx.h"
#include "math/Quaternion.h"

#include <Defs.h>

#include <IO/Input.h>

namespace world
{
	struct Camera
	{
		//view
		math::vec3f pos, fwd, right, up;

		//projection
		float l, r, t, b;
		float n;
		float f;
		float fov;
		float fov_tan;
	};

	inline Camera
	camera_new()
	{
		Camera self{};
		self.pos = math::vec3f{0, 0, 5};
		self.fwd = math::vec3f{ 0,0,-1 };
		self.right = math::vec3f{ 1,0,0 };
		self.up = math::vec3f{0, 1, 0};
		self.l = -1;
		self.r = 1;
		self.b = -1;
		self.t = 1;
		self.n = 0.1f;
		self.f = 1000;
		self.fov = 0.785398185f;
		self.fov_tan = tan(self.fov / 2.0f);

		return self;
	}

	inline void
	camera_viewport(Camera& self, const math::vec2f& viewport)
	{
		self.l = -viewport[0] / 2;
		self.r = viewport[0] + self.l;
		self.b = -viewport[1] / 2;
		self.t = viewport[1] + self.b;
	}

	inline math::Mat4f
	camera_view_matrix(const Camera& self)
	{
		return math::view_matrix(self.fwd, self.right, self.up, self.pos);
	}

	inline math::Mat4f
	camera_lookat(Camera& self, const math::vec3f& eye, const math::vec3f& target, const math::vec3f& up)
	{
		self.pos = eye;
		self.fwd = math::normalize(target - eye);
		self.right = math::normalize(math::cross(self.fwd, up));
		self.up = math::normalize(math::cross(self.right, self.fwd));
		return math::view_matrix(self.fwd, self.right, self.up, self.pos);
	}

	inline math::Mat4f
	camera_proj_matrix(const Camera& self)
	{
		return math::proj_prespective_matrix(self.f, self.n, self.r, self.l, self.t, self.b, self.fov_tan);
	}

	inline math::Mat4f
	camera_view_proj(const Camera& self)
	{
		return camera_proj_matrix(self) * camera_view_matrix(self);
	}

	inline math::vec2f
	camera_viewport(const Camera& self)
	{
		return math::vec2f{self.r-self.l, self.t-self.b};
	}

	inline void
	camera_rotate(Camera& self, const math::vec2f& mouse_delta)
	{
		//test with lookat
		float hangle = -to_radian(mouse_delta[0]) / 10.0f;
		float vangle = to_radian(mouse_delta[1]) / 10.0f;
		math::Quat quath = math::quat_from_axis(self.up, hangle);
		self.fwd = quath * self.fwd;
		self.right = quath * self.right;

		math::Quat quatv = math::quat_from_axis(self.right, vangle);
		self.fwd = quatv * self.fwd;
		self.up = quatv * self.up;
	}

	inline void
	camera_move(Camera& self, const bool keys[], float delta)
	{
		if (keys['w'])
			self.pos += self.fwd *delta;

		if (keys['s'])
			self.pos -= self.fwd* delta;

		if (keys['d'])
			self.pos += self.right * delta;

		if (keys['a'])
			self.pos -= self.right * delta;
	}

	inline void
	camera_zoom(Camera& self, int scroll_offset)
	{
		if (scroll_offset > 0)
			self.fov -= 0.05f;
		else
			self.fov += 0.05f;

		//45 degree
		if (self.fov <= 0.0174533f)
			self.fov = 0.0174533f;
		if (self.fov >= 0.785398185f)
			self.fov = 0.785398185f;

		self.fov_tan = tan(self.fov / 2.0f);
	}
};

