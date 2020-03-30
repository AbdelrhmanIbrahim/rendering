#pragma once

#include <vector>

#include "io/STL_Reader.h"

#include "gl/glgpu.h"

#include "world/components/Vertex.h"

namespace world
{
	struct _Mesh
	{
		//cpu
		std::vector<world::Vertex> vertices;
		std::vector<unsigned int> indices;

		//gpu
		glgpu::Vao va;
		glgpu::Buffer vs, is;
	};

	inline static world::_Mesh
		_mesh_create(const char* stl_path)
	{
		world::_Mesh self{};

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
		_mesh_delete(const world::_Mesh& mesh)
	{
		buffer_delete(mesh.vs);
		buffer_delete(mesh.is);
		vao_delete(mesh.va);
	}
};