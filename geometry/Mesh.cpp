#include "geometry/Mesh.h"
#include "IO/STL_Reader.h"

using namespace glgpu;
using namespace io;
using namespace math;

namespace geo
{
	Mesh
	mesh_create(const char* stl_path)
	{
		Mesh self{};

		//cpu
		Indexed_Triangles tris = stl_binary_read(stl_path);
		self.vertices = std::move(tris.vertices);
		self.indices = std::move(tris.indices);

		//gpu
		self.va = vao_create();
		self.vs = buffer_vertex_create(&self.vertices.front(), self.vertices.size());
		self.is = buffer_index_create(&self.indices.front(), self.indices.size());

		return self;
	}

	void
	mesh_delete(const Mesh& mesh)
	{
		buffer_delete(mesh.vs);
		buffer_delete(mesh.is);
		vao_delete(mesh.va);
	}
}