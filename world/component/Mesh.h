#pragma once

#include <vector>

#include "world/component/Vertex.h"

#include "io/STL_Reader.h"

#include "gl/glgpu.h"

namespace world
{
	struct Mesh
	{
		//cpu
		std::vector<world::Vertex> vertices;
		std::vector<unsigned int> indices;

		//gpu
		glgpu::Vao va;
		glgpu::Buffer vs, is;

		void
		free()
		{
			glgpu::buffer_delete(vs);
			glgpu::buffer_delete(is);
			glgpu::vao_delete(va);
		}
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
		self.vs = glgpu::buffer_vertex_create();
		glgpu::buffer_vertex_set(self.vs, &self.vertices.front(), self.vertices.size());
		self.is = glgpu::buffer_index_create(&self.indices.front(), self.indices.size());
		self.va = vao_create(self.vs, self.is);

		return self;
	}
};