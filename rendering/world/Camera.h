#pragma once

#include "math/Transform.h"
#include "math/Matrix.h"
#include "math/Gfx.h"

namespace world
{
	struct Camera
	{
		//view
		math::Transform view;

		//projection
		float left, right, top, bottom;
		float near;
		float far;
		float fov;
	};

	inline math::Mat4f
	camera_view_matrix(const Camera& cam)
	{
		return math::mat4_from_transform(cam.view);
	}

	inline math::Mat4f
	camera_proj_matrix(const Camera& cam)
	{
		math::Mat4f proj{};
		float fov = tan(cam.fov / 2.0);
		float distance = cam.far - cam.near;
		float aspect_ratio = cam.right - cam.left / cam.top - cam.bottom;
		proj[0][0] = 1.0f / (aspect_ratio*fov); 
		proj[1][1] = 1.0f / fov;
		proj[2][2] = -(cam.near + cam.far) / distance;
		proj[2][3] = -1.0f;
		proj[3][2] = -2.0f*cam.far*cam.near / distance;

		return proj;
};	}
