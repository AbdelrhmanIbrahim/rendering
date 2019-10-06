#pragma once

#include "geometry/vertex.h"
#include "gpu_gl/glgpu.h"

namespace geo
{
	struct Mesh
	{
		//cpu
		geo::Vertex vertices[3];
		unsigned int indices[3];

		//gpu
		glgpu::vao va;
		glgpu::buffer vs, is;
	};

	Mesh
	mesh_create(geo::Vertex vs[], unsigned int is[]);

	void
	mesh_delete(const Mesh& mesh);

	void
	mesh_draw(const Mesh& mesh);
}