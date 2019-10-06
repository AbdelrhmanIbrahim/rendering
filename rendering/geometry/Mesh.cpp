#include "geometry/Mesh.h"

using namespace glgpu;

namespace geo
{
	Mesh
	mesh_create(geo::Vertex vs[], unsigned int is[])
	{
		Mesh self{};
		self.vertices[0] = vs[0];
		self.vertices[1] = vs[1];
		self.vertices[2] = vs[2];
		self.indices[0] = is[0];
		self.indices[1] = is[1];
		self.indices[2] = is[2];

		self.vs = vertex_buffer_create(self.vertices);
		self.is = index_buffer_create(self.indices);
		self.va = vao_create();

		return self;
	}

	void
	mesh_delete(const Mesh& mesh)
	{
		buffer_delete(mesh.vs);
		buffer_delete(mesh.is);
		vao_delete(mesh.va);
	}

	void
	mesh_draw(const Mesh& mesh)
	{
		vao_bind(mesh.va, mesh.vs, mesh.is);
		draw_indexed(sizeof(mesh.indices));
		vao_unbind();
	}
}