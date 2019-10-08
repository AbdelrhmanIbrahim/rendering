#pragma once

#include "geometry/vertex.h"

#define HANDLE(NAME) typedef struct NAME##__ { int unused; } *NAME;

namespace glgpu
{
	//make an internal handle instead of casting to an address ofc
	HANDLE(program);
	HANDLE(buffer);
	HANDLE(vao);

	void
	graphics_init();

	program
	program_create(const char* vertex_shader_path, const char* pixel_shader_path);

	void
	program_use(program prog);

	void
	program_delete(program prog);

	buffer
	vertex_buffer_create(geo::Vertex vertices[]);

	buffer
	index_buffer_create(unsigned int indices[]);

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

	void
	color_clear(float r, float g, float b);

	void
	draw_indexed(unsigned int indcies_count);

	bool
	error();
}