#include "engine/renderer/Edge.h"

#include "world/component/Camera.h"
#include "world/component/Mesh.h"
#include "world/component/Transform.h"

#include "math/Matrix.h"

#include "gl/glgpu.h"

using namespace math;
using namespace glgpu;

namespace rndr
{
	struct IEdge
	{
		glgpu::Program prog;
		glgpu::Buffer uniform_space;
		glgpu::Buffer uniform_edge_color;
	};

	//Careful of std140 memory layout/alignment/offsets.prefer always to allocate chunks of 16 bytes.
	//Always check renderdoc.
	struct Space_Uniform
	{
		Mat4f mvp;
	};

	Edge
	edge_create()
	{
		IEdge* self = new IEdge();

		//TODO, deploy shaders to bin when moving to cmake or create a res obj (revisit)
		self->prog = program_create(DIR_PATH"/src/engine/shaders/edge.vertex", DIR_PATH"/src/engine/shaders/edge.pixel");
		self->uniform_space = buffer_uniform_create(sizeof(Space_Uniform));
		self->uniform_edge_color = buffer_uniform_create(sizeof(Vec4f));

		return self;
	}

	void
	edge_free(Edge self)
	{
		handle_free(self->prog);
		handle_free(self->uniform_space);
		handle_free(self->uniform_edge_color);

		delete self;
	}

	void
	edge_set(Edge self, math::Vec2f viewport)
	{
		program_use(self->prog);
		buffer_uniform_bind(self->uniform_space, 0);
		buffer_uniform_bind(self->uniform_edge_color, 2);
		view_port(0, 0, (int)viewport[0], (int)viewport[1]);
	}

	void
	edge_draw(Edge self, const math::Mat4f& view_proj, const world::Mesh* mesh, const world::Transform* model, const math::Vec4f& col)
	{
		//uniform blocks
		Space_Uniform mvp{ view_proj * mat4_from_transform(*model) };
		buffer_uniform_set(self->uniform_space, &mvp, sizeof(mvp));
		buffer_uniform_set(self->uniform_edge_color, (void*)&col, sizeof(col));

		//draw geometry
		poly_edges_enable();
		vao_bind(mesh->vao);
		draw_indexed(mesh->indices.size());
		vao_unbind();
		poly_edges_disable();
	}
};