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
	_cube_test()
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
		mesh.vs = glgpu::buffer_vertex_create(&mesh.vertices.front(), (unsigned int)mesh.vertices.size());
		mesh.va = glgpu::vao_create(mesh.vs);
		return mesh;
	}

	inline geo::Mesh
	_sphere_create()
	{
		geo::Mesh mesh{};
		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359;
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = cos(xSegment * 2.0f * PI) * sin(ySegment * PI);
				float yPos = cos(ySegment * PI);
				float zPos = sin(xSegment * 2.0f * PI) *sin(ySegment * PI);
				mesh.vertices.push_back(geo::Vertex{ math::vec3f{xPos, yPos, zPos}, math::vec3f{xPos, yPos, zPos}, math::vec2f{xSegment, ySegment} });
			}
		}
		bool oddRow = false;
		for (int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (int x = 0; x <= X_SEGMENTS; ++x)
				{
					mesh.indices.push_back(y       * (X_SEGMENTS + 1) + x);
					mesh.indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					mesh.indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					mesh.indices.push_back(y       * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}

		mesh.vs = glgpu::buffer_vertex_create(&mesh.vertices.front(), (unsigned int)mesh.vertices.size());
		mesh.is = glgpu::buffer_index_create(&mesh.indices.front(), mesh.indices.size());
		mesh.va = glgpu::vao_create(mesh.vs, mesh.is);
		return mesh;
	}

	inline object3D
	object3d_create(const math::vec3f& rot_axis, float angle, const math::vec3f& scale, const math::vec3f& translate, const char* path)
	{
		object3D self{};
		self.model = math::Transform{ rot_axis, angle, scale, translate };
		self.mesh = geo::mesh_create(path);
		return self;
	}

	inline void
	object3d_delete(object3D& obj)
	{
		geo::mesh_delete(obj.mesh);
	}
};