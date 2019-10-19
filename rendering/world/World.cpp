#include "world/World.h"
#include "world/3Dobject.h"

using namespace math;

namespace world
{
	/*Mesh
	_mesh_test()
	{
		Mesh mesh{};
		mesh.vertices.push_back(geo::Vertex{ 0.9f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f });
		mesh.vertices.push_back(geo::Vertex{ 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, });
		mesh.vertices.push_back(geo::Vertex{ -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, });
		mesh.vertices.push_back(geo::Vertex{ -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f });
		mesh.indices.push_back(0);
		mesh.indices.push_back(1);
		mesh.indices.push_back(2);
		mesh.indices.push_back(0);
		mesh.indices.push_back(2);
		mesh.indices.push_back(3);
		mesh.vs = glgpu::vertex_buffer_create(&mesh.vertices.front(), mesh.vertices.size());
		mesh.is = glgpu::index_buffer_create(&mesh.indices.front(), mesh.indices.size());
		mesh.va = glgpu::vao_create();
		return mesh;
	}*/

	World*
	world_create()
	{
		World* self = new World;
		self->obj = object3d_create(math::Z_AXIS, 0.785f, vec3f{1.0f, 1.0f, 1.0f}, vec3f{}, "../rendering/res/stls/cube.stl");
		return self;
	}

	void
	world_free(World* w)
	{
		//revisit
		object3d_delete(w->obj);
		delete w;
	}
}