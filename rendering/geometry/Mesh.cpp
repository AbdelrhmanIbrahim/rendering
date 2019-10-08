#include "geometry/Mesh.h"
#include "IO/STL_Reader.h"

using namespace glgpu;
using namespace io;

namespace geo
{
	Mesh
	mesh_create(const char* stl_path)
	{
		Mesh self{};
		Indexed_Triangles tris = stl_binary_read(stl_path);
		self.vertices = std::move(tris.vertices);
		self.indices = std::move(tris.indices);
		self.vs = vertex_buffer_create(&self.vertices.front(), self.vertices.size());
		self.is = index_buffer_create(&self.indices.front(), self.indices.size());
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