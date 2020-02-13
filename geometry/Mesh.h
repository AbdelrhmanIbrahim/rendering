#pragma once

#include "geometry/vertex.h"
#include "gpu_gl/glgpu.h"

#include <vector>

namespace geo
{
	struct Mesh
	{
		//cpu
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		//gpu
		glgpu::vao va;
		glgpu::buffer vs, is;
	};

	Mesh
	mesh_create(const char* stl_path);

	void
	mesh_delete(const Mesh& mesh);
}