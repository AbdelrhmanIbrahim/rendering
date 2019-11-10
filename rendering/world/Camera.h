#pragma once

#include "math/Vector.h"
#include "math/Matrix.h"
#include "math/Gfx.h"

namespace world
{
	struct Camera
	{
		//view
		math::vec3f pos, fwd, right, up;
		//float pitch, yaw;

		//projection
		float l, r, t, b;
		float n;
		float f;
		float fov;
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
		self.n = 0.1;
		self.f = 100;
		self.fov = 0.785398185;
		self.pitch= 0;
		self.yaw = 0;

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
		math::Mat4f view{};
		view[0] = math::vec4f{ self.right[0], self.up[0], -self.fwd[0], 0 };
		view[1] = math::vec4f{ self.right[1], self.up[1], -self.fwd[1], 0 };
		view[2] = math::vec4f{ self.right[2], self.up[2], -self.fwd[2], 0 };
		view[3] = math::vec4f{ -dot(self.pos, self.right), -dot(self.pos, self.up), dot(self.pos, self.fwd), 1 };

		return view;
	}

	inline math::Mat4f
	camera_proj_matrix(const Camera& self)
	{
		math::Mat4f proj{};
		float distance = self.f - self.n;
		float aspect_ratio = (self.r - self.l) / (self.t - self.b);
		float tan_fov = tan(self.fov / 2.0f);
		proj[0][0] = 1.0f / (aspect_ratio * tan_fov);
		proj[1][1] = 1.0f / tan_fov;
		proj[2][2] = -(self.n + self.f) / distance;
		proj[2][3] = -1.0f;
		proj[3][2] = -2.0f*self.f*self.n / distance;

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
		return math::vec2f{self.r-self.l, self.t-self.b};
	}

	inline void
	camera_rotate(Camera& self, const math::vec2f& mouse_delta)
	{
		/*self.yaw += mouse_delta[1];
		self.pitch += mouse_delta[0];

		if (self.pitch > 89.0f)
			self.pitch = 89.0f;
		if (self.pitch < -89.0f)
			self.pitch = -89.0f;

		self.fwd[0] = cos(self.yaw * 3.14 /180) * cos(self.pitch* 3.14 / 180);
		self.fwd[1] = sin(self.pitch* 3.14 / 180);
		self.fwd[2] = sin(self.yaw* 3.14 / 180) * cos(self.pitch* 3.14 / 180);
		self.fwd = math::normalize(self.fwd);
		self.right = math::cross(self.fwd, self.up);
		self.up = math::cross(self.right, self.fwd);*/

		float hangle = -math::PI * mouse_delta[0] / 1800.0f;
		float vangle = -math::PI * mouse_delta[1] / 1800.0f;
		//simplify to quadternions or to yaw-pitch
		self.fwd = math::mat4f_rotate(self.up, hangle) * math::vec4f { self.fwd[0], self.fwd[1], self.fwd[2], 0.0f };
		self.right = math::mat4f_rotate(self.up, hangle) * math::vec4f{ self.right[0], self.right[1], self.right[2], 0.0f };
		self.fwd = math::mat4f_rotate(self.right, vangle) * math::vec4f{ self.fwd[0], self.fwd[1], self.fwd[2], 0.0f };
		self.up = math::mat4f_rotate(self.right, vangle) * math::vec4f{ self.up[0], self.up[1], self.up[2], 0.0f };
		self.right = math::normalize(math::cross(self.fwd, self.up));
	}

	inline void
	camera_move(Camera& self, unsigned char c, float delta)
	{
		switch (c)
		{
		case 'w':
			self.pos += self.fwd *delta;
			break;

		case 's':
			self.pos -= self.fwd* delta;
			break;

		case 'd':
			self.pos += self.right * delta;
			break;

		case 'a':
			self.pos -= self.right * delta;
			break;

		default:
			break;
		}
	}
};

