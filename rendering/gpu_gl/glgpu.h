#pragma once

#include "geometry/vertex.h"
#include "math/Vector.h"
#include "math/Matrix.h"

#define HANDLE(NAME) typedef struct NAME##__ { int unused; } *NAME;

namespace glgpu
{
	//make an internal handle instead of casting to an address ofc
	HANDLE(program);
	HANDLE(buffer);
	HANDLE(texture);
	HANDLE(vao);

	enum TEXTURE_UNIT
	{
		UNIT_0
	};

	void
	graphics_init();

	program
	program_create(const char* vertex_shader_path, const char* pixel_shader_path);

	void
	program_use(program prog);

	void
	program_delete(program prog);

	buffer
	vertex_buffer_create(geo::Vertex vertices[], unsigned int count);

	buffer
	index_buffer_create(unsigned int indices[], unsigned int count);

	void
	buffer_delete(buffer buf);

	vao
	vao_create();

	void
	vao_bind(vao v, buffer vbo, buffer ebo);

	void
	vao_unbind();

	void
	vao_delete(vao va);

	texture
	texture_create(const char* image_path);

	void
	texture_bind(texture texture, TEXTURE_UNIT texture_unit);

	void
	texture_free(texture texture);

	void
	color_clear(float r, float g, float b);

	void
	draw_strip(unsigned int vertices_count);

	void
	draw_indexed(unsigned int indcies_count);

	void
	uniform4f_set(program prog, const char* uniform, math::vec4f& data);

	void
	uniformmat4f_set(program prog, const char* uniform, math::Mat4f& data);

	void
	uniform1i_set(program prog, const char* uniform, int data);

	void
	view_port(int x, int y, int width, int height);

	bool
	error();
}