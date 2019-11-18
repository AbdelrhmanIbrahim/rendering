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
	
	inline geo::Mesh
	_mesh_test()
	{
		geo::Mesh mesh{};
		mesh.vertices.push_back(geo::Vertex{-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f});
		mesh.vertices.push_back(geo::Vertex{ 0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f});
		mesh.vertices.push_back(geo::Vertex{ 0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f});
		mesh.vertices.push_back(geo::Vertex{ 0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f});
		mesh.vertices.push_back(geo::Vertex{-0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 1.0f});
		mesh.vertices.push_back(geo::Vertex{-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f});

		mesh.vertices.push_back(geo::Vertex{-0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f});
		mesh.vertices.push_back(geo::Vertex{ 0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f, 0.0f});
		mesh.vertices.push_back(geo::Vertex{ 0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f, 1.0f});
		mesh.vertices.push_back(geo::Vertex{ 0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f, 1.0f});
		mesh.vertices.push_back(geo::Vertex{-0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f});
		mesh.vertices.push_back(geo::Vertex{-0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f});

		mesh.vertices.push_back(geo::Vertex{-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f});
		mesh.vertices.push_back(geo::Vertex{-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f});
		mesh.vertices.push_back(geo::Vertex{-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f});
		mesh.vertices.push_back(geo::Vertex{-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f});
		mesh.vertices.push_back(geo::Vertex{-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f});
		mesh.vertices.push_back(geo::Vertex{-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f});

		mesh.vertices.push_back(geo::Vertex{ 0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f});
		mesh.vertices.push_back(geo::Vertex{ 0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 1.0f});
		mesh.vertices.push_back(geo::Vertex{ 0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f});
		mesh.vertices.push_back(geo::Vertex{ 0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f});
		mesh.vertices.push_back(geo::Vertex{ 0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f});
		mesh.vertices.push_back(geo::Vertex{ 0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f});

		mesh.vertices.push_back(geo::Vertex{-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f});
		mesh.vertices.push_back(geo::Vertex{ 0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f});
		mesh.vertices.push_back(geo::Vertex{ 0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f});
		mesh.vertices.push_back(geo::Vertex{ 0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f});
		mesh.vertices.push_back(geo::Vertex{-0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f});
		mesh.vertices.push_back(geo::Vertex{-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f});

		mesh.vertices.push_back(geo::Vertex{-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f});
		mesh.vertices.push_back(geo::Vertex{ 0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f});
		mesh.vertices.push_back(geo::Vertex{ 0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f});
		mesh.vertices.push_back(geo::Vertex{ 0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f});
		mesh.vertices.push_back(geo::Vertex{-0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f});
		mesh.vertices.push_back(geo::Vertex{-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f});
		mesh.vs = glgpu::vertex_buffer_create(&mesh.vertices.front(), (unsigned int)mesh.vertices.size());
		mesh.va = glgpu::vao_create();
		return mesh;
	}

	inline object3D
	object3d_create(const math::vec3f& rot_axis, float angle, const math::vec3f& scale, const math::vec3f& translate, const char* path)
	{
		object3D self{};
		self.model = math::Transform{ rot_axis, angle, scale, translate };
		self.mesh = _mesh_test();
		return self;
	}

	inline void
	object3d_delete(const object3D& obj)
	{
		geo::mesh_delete(obj.mesh);
	}
};