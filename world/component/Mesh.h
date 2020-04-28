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
		self.vs = glgpu::buffer_vertex_create(&self.vertices.front(), self.vertices.size());
		self.is = glgpu::buffer_index_create(&self.indices.front(), self.indices.size());
		self.va = glgpu::vao_create();
		glgpu::vao_attach(self.va, self.vs, self.is);
		glgpu::buffer_vertex_attribute(self.vs, 0, 3, sizeof(world::Vertex), 0);
		glgpu::buffer_vertex_attribute(self.vs, 1, 3, sizeof(world::Vertex), 3 * sizeof(float));
		glgpu::buffer_vertex_attribute(self.vs, 2, 2, sizeof(world::Vertex), 6 * sizeof(float));
		glgpu::vao_unbind();

		return self;
	}
};