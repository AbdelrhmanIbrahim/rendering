#pragma once

#include <vector>

#include "io/STL_Reader.h"

#include "geometry/Vertex.h"

#include "gl/glgpu.h"

namespace world
{
	struct Mesh
	{
		//cpu
		std::vector<geo::Vertex> vertices;
		std::vector<unsigned int> indices;

		//gpu
		glgpu::Vao va;
		glgpu::Buffer vs, is;
	};

	inline static world::Mesh
	mesh_create(const char* stl_path)
	{
		world::Mesh self{};

		//cpu
		io::Indexed_Triangles tris = io::stl_binary_read(stl_path);
		self.vertices = std::move(tris.vertices);
		self.indices = std::move(tris.indices);

		//gpu
		self.vs = glgpu::buffer_vertex_create(&self.vertices.front(), self.vertices.size());
		self.is = glgpu::buffer_index_create(&self.indices.front(), self.indices.size());
		self.va = vao_create(self.vs, self.is);

		return self;
	}

	inline static void
	mesh_delete(const world::Mesh& mesh)
	{
		buffer_delete(mesh.vs);
		buffer_delete(mesh.is);
		vao_delete(mesh.va);
	}
};