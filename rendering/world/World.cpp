#include "world/World.h"

using namespace geo;

namespace world
{
	Mesh
	_mesh_test()
	{
		Mesh mesh{};
		mesh.vertices.push_back(geo::Vertex{ 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f });
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
	}

	World*
	world_create()
	{
		World* self = new World;
		/*
		self->mesh = mesh_create("../rendering/res/stls/cube.stl");
		self->mesh.vertices[0].uv = math::vec2f{ 0,0 };
		self->mesh.vertices[1].uv = math::vec2f{ 1,0 };
		self->mesh.vertices[2].uv = math::vec2f{ 1,1 };
		self->mesh.vertices[3].uv = math::vec2f{ 0,1 };
		self->mesh.vertices[4].uv = math::vec2f{ 0,0 };
		self->mesh.vertices[5].uv = math::vec2f{ 1,0 };
		self->mesh.vertices[6].uv = math::vec2f{ 1,1 };
		self->mesh.vertices[7].uv = math::vec2f{ 0,1 };
		*/

		self->mesh = _mesh_test();
		return self;
	}

	void
	world_free(World* w)
	{
		//revisit
		mesh_delete(w->mesh);
		delete w;
	}
}