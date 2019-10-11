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
		Indexed_Triangles tris = stl_binary_read(stl_path);
		self.vertices = std::move(tris.vertices);
		self.indices = std::move(tris.indices);

		//revisit -- manually set uvs, glow the texture to each rectangle
		static vec2f uvs[4] = { math::vec2f{ 0,0 }, math::vec2f{ 1,0 }, math::vec2f{ 1,1 }, math::vec2f{ 0,1 } };
		unsigned int index = 0;
		for (auto& vertex : self.vertices)
			vertex.uv = uvs[index++];

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
}