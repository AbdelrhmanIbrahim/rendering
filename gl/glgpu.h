#pragma once

#include "math/Vector.h"
#include "math/Matrix.h"

#include "io/Image.h"

#include "defs/Defs.h"

#include <cstddef>

namespace glgpu
{
	typedef struct IGL_Handle* Vao;
	typedef struct IGL_Handle* Buffer;
	typedef struct IGL_Handle* Program;
	typedef struct IGL_Handle* Texture;
	typedef struct IGL_Handle* Sampler;
	typedef struct IGL_Handle* Framebuffer;
	typedef struct IGL_Handle* Cubemap;

	void
	graphics_init();

	Program
	program_create(const char* vertex_shader_path, const char* pixel_shader_path);

	Program
	program_create(const char* vertex_shader_path, const char* geometry_shader_path, const char* pixel_shader_path);

	void
	program_use(Program prog);

	void
	program_delete(Program prog);

	Buffer
	buffer_vertex_create();

	void
	buffer_vertex_set(Buffer self, const void* arr, int total_size_bytes, Storage storage);

	void
	buffer_vertex_attribute(Buffer self, int att, int att_components_number, int size_bytes, int offset);

	void
	buffer_vertex_bind(Buffer self);

	void
	buffer_vertex_unbind();

	Buffer
	buffer_index_create(unsigned int indices[], std::size_t count);

	Buffer
	buffer_uniform_create(unsigned int size_in_bytes);

	void
	buffer_uniform_bind(unsigned int binding_point, Buffer data);

	void
	buffer_uniform_set(Buffer buf, void* data, unsigned int size_in_bytes);

	void
	buffer_delete(Buffer buf);

	Vao
	vao_create();

	void
	vao_bind(Vao vao);

	void
	vao_unbind();

	void
	vao_attach(Vao vao, Buffer vbo);

	void
	vao_attach(Vao vao, Buffer vbo, Buffer ibo);

	void
	vao_delete(Vao va);

	Texture
	texture2d_create(math::vec2f size, INTERNAL_TEXTURE_FORMAT internal_format, EXTERNAL_TEXTURE_FORMAT format, DATA_TYPE type, bool mipmap);

	Texture
	texture2d_create(const io::Image& img, io::IMAGE_FORMAT format);

	Texture
	texture2d_create(const char* image_path, io::IMAGE_FORMAT format);

	void
	texture2d_render_offline_to(Texture output, Program prog, math::vec2f view_size);

	void
	texture2d_bind(Texture texture, unsigned int texture_unit_index);

	void
	texture2d_unbind();

	void
	texture2d_unpack(Texture texture, io::Image& image, EXTERNAL_TEXTURE_FORMAT format, DATA_TYPE type);

	void
	texture_free(Texture texture);

	Sampler
	sampler_create(TEXTURE_FILTERING min_filtering, TEXTURE_FILTERING mag_filtering, TEXTURE_SAMPLING sampling);

	void
	sampler_bind(Sampler self, unsigned int texture_unit);

	void
	sampler_free(Sampler self);

	Cubemap
	cubemap_create(math::vec2f view_size, INTERNAL_TEXTURE_FORMAT texture_format, EXTERNAL_TEXTURE_FORMAT ext_format, DATA_TYPE type, bool mipmap);

	Cubemap
	cubemap_rgba_create(const io::Image imgs[6]);

	Cubemap
	cubemap_hdr_create(const io::Image& img, math::vec2f view_size, bool mipmap);

	//TODO -- ability to send array of different uniforms types table (revisit)
	void
	cubemap_postprocess(Cubemap input, Cubemap output, Program postprocessor, Unifrom_Float uniform, math::vec2f view_size, int mipmap_level);

	void
	cubemap_bind(Cubemap texture, unsigned int texture_unit);

	void
	cubemap_free(Cubemap cmap);

	Framebuffer
	framebuffer_create();

	void
	framebuffer_bind(Framebuffer fb);

	void
	framebuffer_attach(Framebuffer fb, Texture tex, FRAMEBUFFER_ATTACHMENT attachment);

	void
	framebuffer_unbind();

	void
	framebuffer_free(Framebuffer fb);

	void
	disable_color_buffer_rw();

	void
	depth_clear();

	void
	frame_start();

	void
	color_clear(float r, float g, float b);

	void
	depth_test(DEPTH_TEST test);

	void
	draw_points(std::size_t vertices_count, float thickness);

	void
	draw_lines(std::size_t vertices_count, float thickness);

	void
	draw_strips(std::size_t vertices_count);

	void
	draw_indexed(unsigned int indcies_count);

	void
	uniform1f_set(Program prog, const char* uniform, float data);

	void
	uniform3f_set(Program prog, const char* uniform, const math::vec3f& data);

	void
	uniform4f_set(Program prog, const char* uniform, const math::vec4f& data);

	void
	uniformmat4f_set(Program prog, const char* uniform, const math::Mat4f& data);

	void
	view_port(int x, int y, int width, int height);

	bool
	error();
}