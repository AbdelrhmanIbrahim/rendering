#pragma once

#include "math/Transform.h"
#include "math/Vector.h"
#include "geometry/Mesh.h"

namespace world
{
	struct object3D
	{
		math::Transform model;
		geo::Mesh mesh;
	};

	inline object3D
	object3d_create(const math::vec3f& rot_axis, float angle, const math::vec3f& scale, const math::vec3f& translate, const char* path)
	{
		object3D self{};
		self.model = math::Transform{ rot_axis, angle, scale, translate };
		self.mesh = geo::mesh_create(path);
		return self;
	}

	inline void
	object3d_delete(const object3D& obj)
	{
		geo::mesh_delete(obj.mesh);
	}
};