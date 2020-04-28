#pragma once

#include <vector>

#include "world/component/Vertex.h"

#include "io/STL_Reader.h"

#include "gl/glgpu.h"

#include "defs/Defs.h"

namespace world
{
	struct Mesh
	{
		//cpu
		std::vector<world::Vertex> vertices;
		std::vector<unsigned int> indices;

		//gpu
		glgpu::Vao vao;
		glgpu::Buffer vbo, ibo;

		void
		free()
		{
			glgpu::buffer_delete(vao);
			glgpu::buffer_delete(ibo);
			glgpu::vao_delete(vao);
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
		self.vbo = glgpu::buffer_vertex_create();
		self.ibo = glgpu::buffer_index_create(&self.indices.front(), self.indices.size());
		self.vao = glgpu::vao_create();

		glgpu::vao_attach(self.vao, self.vbo, self.ibo);
		glgpu::buffer_vertex_set(self.vbo, &self.vertices.front(), self.vertices.size(), Storage::STATIC);
		glgpu::buffer_vertex_attribute(self.vbo, 0, 3, sizeof(world::Vertex), 0);
		glgpu::buffer_vertex_attribute(self.vbo, 1, 3, sizeof(world::Vertex), 3 * sizeof(float));
		glgpu::buffer_vertex_attribute(self.vbo, 2, 2, sizeof(world::Vertex), 6 * sizeof(float));
		glgpu::vao_unbind();

		return self;
	}
};