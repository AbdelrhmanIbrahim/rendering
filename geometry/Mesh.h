#pragma once

#include "geometry/Vertex.h"
#include "gl/glgpu.h"

#include <vector>

namespace geo
{
	struct Mesh
	{
		//cpu
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		//gpu
		glgpu::Vao va;
		glgpu::Buffer vs, is;
	};

	Mesh
	mesh_create(const char* stl_path);

	void
	mesh_delete(const Mesh& mesh);
}