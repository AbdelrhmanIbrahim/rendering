#include "gl/glgpu.h"
#include "gl/glew.h"

#include "world/component/Vertex.h"

#include "math/Matrix.h"
#include "math/Gfx.h"

#include <assert.h>
#include <fstream>
#include <string>

using namespace io;
using namespace math;
using namespace world;

namespace glgpu
{
	//unit cube can be used in some algorithms like equirectangle mapping, irradiance map, etc..
	constexpr static Vertex unit_cube[36] =
	{
		//back
		Vertex{-1.0f, -1.0f, -1.0f},
		Vertex{1.0f, 1.0f, -1.0f},
		Vertex{1.0f, -1.0f, -1.0f},
		Vertex{1.0f, 1.0f, -1.0f},
		Vertex{-1.0f, -1.0f, -1.0f},
		Vertex{-1.0f, 1.0f, -1.0},

		//front
		Vertex{-1.0f, -1.0f, 1.0},
		Vertex{1.0f, -1.0f, 1.0f},
		Vertex{1.0f, 1.0f, 1.0f,},
		Vertex{1.0f, 1.0f, 1.0f,},
		Vertex{-1.0f, 1.0f, 1.0f},
		Vertex{-1.0f, -1.0f, 1.0},

		//left
		Vertex{-1.0f, 1.0f, 1.0f},
		Vertex{-1.0f, 1.0f, -1.0f},
		Vertex{-1.0f, -1.0f, -1.0f},
		Vertex{-1.0f, -1.0f, -1.0f},
		Vertex{-1.0f, -1.0f, 1.0},
		Vertex{-1.0f, 1.0f, 1.0f},

		//right
		Vertex{1.0f, 1.0f, 1.0f,},
		Vertex{1.0f, -1.0f, -1.0},
		Vertex{1.0f, 1.0f, -1.0f},
		Vertex{1.0f, -1.0f, -1.0f},
		Vertex{1.0f, 1.0f, 1.0f,},
		Vertex{1.0f, -1.0f, 1.0f},

		//bottom
		Vertex{-1.0f, -1.0f, -1.0f},
		Vertex{1.0f, -1.0f, -1.0f},
		Vertex{1.0f, -1.0f, 1.0f},
		Vertex{1.0f, -1.0f, 1.0f},
		Vertex{-1.0f, -1.0f, 1.0f},
		Vertex{-1.0f, -1.0f, -1.0f},

		//top
		Vertex{-1.0f, 1.0f, -1.0f},
		Vertex{1.0f, 1.0f, 1.0f,},
		Vertex{1.0f, 1.0f, -1.0f},
		Vertex{1.0f, 1.0f, 1.0f,},
		Vertex{-1.0f, 1.0f, -1.0f},
		Vertex{-1.0f, 1.0f, 1.0f}
	};

	constexpr static Vertex quad_ndc[6] =
	{
		Vertex{-1.0f,  1.0f, 0.0f,  vec3f{0,0,1}, 0.0f, 1.0f},
		Vertex{-1.0f, -1.0f, 0.0f,  vec3f{0,0,1}, 0.0f, 0.0f},
		Vertex{ 1.0f,  1.0f, 0.0f,  vec3f{0,0,1}, 1.0f, 1.0f},

		Vertex{ 1.0f,  1.0f, 0.0f,  vec3f{0,0,1}, 1.0f, 1.0f},
		Vertex{-1.0f, -1.0f, 0.0f,  vec3f{0,0,1}, 0.0f, 0.0f},
		Vertex{ 1.0f, -1.0f, 0.0f,  vec3f{0,0,1}, 1.0f, 0.0f}
	};

	struct IGL_Handle
	{
		enum KIND
		{
			KIND_VAO,
			KIND_BUFFER,
			KIND_PROGRAM,
			KIND_SAMPLER,
			KIND_TEXTURE,
			KIND_CUBEMAP,
			KIND_FRAMEBUFFER
		};

		KIND kind;

		union
		{
			struct
			{
				GLuint id;
			} vao;

			struct
			{
				GLuint id;
				GLenum type;
			} buffer;

			struct
			{
				GLuint id;
			} program;

			struct
			{
				GLuint id;
				GLenum min_filtering, mag_filtering;
				GLenum sampling;
			} sampler;

			struct
			{
				//remove filtering and sampling parameters from textures, we have samplers now
				GLuint id;
				unsigned int width, height;
				GLenum internal_format;
				GLenum type;
				GLenum pixel_format;
			} texture;

			struct
			{
				//remove filtering and sampling parameters from textures, we have samplers now
				GLuint id;
				unsigned int width, height;
				GLenum internal_format;
				GLenum type;
				GLenum pixel_format;
			} cubemap;

			struct
			{
				GLuint id;
			} framebuffer;
		};
	};

	void
	graphics_init()
	{
		GLenum gl_ok = glewInit();
		assert(gl_ok == GLEW_OK);
	}

	enum class SHADER_STAGE
	{
		VERTEX,
		GEOMETRY,
		PIXEL
	};

	//do we really need all this function name mangling to avoid a longer function name? nah (TODO)
	GLenum
	_map(DEPTH_TEST test)
	{
		switch (test)
		{
		case DEPTH_TEST::LE:
			return GL_LEQUAL;
		case DEPTH_TEST::L:
			return GL_LESS;
		case DEPTH_TEST::G:
			return GL_GREATER;
		default:
			assert("undefined depth test" && false);
			return -1;
		}
	}

	GLenum
	_map(SHADER_STAGE stage)
	{
		switch (stage)
		{
		case SHADER_STAGE::VERTEX:
			return GL_VERTEX_SHADER;
		case SHADER_STAGE::GEOMETRY:
			return GL_GEOMETRY_SHADER;
		case SHADER_STAGE::PIXEL:
			return GL_FRAGMENT_SHADER;
		default:
			assert("undefined shader stage" && false);
			return -1;
		}
	}

	GLenum
	_map(FRAMEBUFFER_ATTACHMENT attachment)
	{
		switch (attachment)
		{
		case FRAMEBUFFER_ATTACHMENT::COLOR0:
			return GL_COLOR_ATTACHMENT0;
		case FRAMEBUFFER_ATTACHMENT::COLOR1:
			return GL_COLOR_ATTACHMENT1;
		case FRAMEBUFFER_ATTACHMENT::DEPTH_STENCIL:
			return GL_DEPTH_STENCIL_ATTACHMENT;
		default:
			assert("undefined fb attachment" && false);
			return -1;
		}
	}

	GLenum
	_map(EXTERNAL_TEXTURE_FORMAT format)
	{
		switch (format)
		{
		case EXTERNAL_TEXTURE_FORMAT::RG:
			return GL_RG;
		case EXTERNAL_TEXTURE_FORMAT::RGB:
			return GL_RGB;
		case EXTERNAL_TEXTURE_FORMAT::RGBA:
			return GL_RGBA;
		case EXTERNAL_TEXTURE_FORMAT::DEPTH_STENCIL:
			return GL_DEPTH_STENCIL;
		default:
			assert("undefined format" && false);
			return -1;
		}
	}

	GLenum
	_map(INTERNAL_TEXTURE_FORMAT format)
	{
		switch (format)
		{
		case INTERNAL_TEXTURE_FORMAT::RG16F:
			return GL_RG16F;
		case INTERNAL_TEXTURE_FORMAT::RGB:
			return GL_RGB;
		case INTERNAL_TEXTURE_FORMAT::RGBA:
			return GL_RGBA;
		case INTERNAL_TEXTURE_FORMAT::RGB16F:
			return GL_RGB16F;
		case INTERNAL_TEXTURE_FORMAT::DEPTH_STENCIL:
			return GL_DEPTH24_STENCIL8;
		default:
			assert("undefined format" && false);
			return -1;
		}
	}

	GLenum
	_map(DATA_TYPE type)
	{
		switch (type)
		{
		case DATA_TYPE::UBYTE:
			return GL_UNSIGNED_BYTE;
		case DATA_TYPE::FLOAT:
			return GL_FLOAT;
		case DATA_TYPE::UINT_24_8:
			return GL_UNSIGNED_INT_24_8;
		default:
			assert("undefined data type" && false);
			return -1;
		}
	}

	GLenum
	_map(TARGET target)
	{
		switch (target)
		{
		case TARGET::TEXTURE_2D:
			return GL_TEXTURE_2D;
		case TARGET::CUBEMAP:
			return GL_TEXTURE_CUBE_MAP;
		default:
			assert("undefined target type" && false);
			return -1;
		}
	}

	GLenum
	_map(TEXTURE_FILTERING filter)
	{
		switch (filter)
		{
		case TEXTURE_FILTERING::LINEAR:
			return GL_LINEAR;
		case TEXTURE_FILTERING::NEAREST:
			return GL_NEAREST;
		case TEXTURE_FILTERING::LINEAR_MIPMAP:
			return GL_LINEAR_MIPMAP_LINEAR;
		default:
			assert("undefined filter type" && false);
			return -1;
		}
	}

	GLenum
	_map(TEXTURE_SAMPLING sampling)
	{
		switch (sampling)
		{
		case TEXTURE_SAMPLING::REPEAT:
			return GL_REPEAT;
		case TEXTURE_SAMPLING::MIRROR_REPEAT:
			return GL_MIRRORED_REPEAT;
		case TEXTURE_SAMPLING::CLAMP_TO_EDGE:
			return GL_CLAMP_TO_EDGE;
		case TEXTURE_SAMPLING::CLAMP_TO_BORDER:
			return GL_CLAMP_TO_BORDER;
		default:
			assert("undefined sampling type" && false);
			return -1;
		}
	}

	GLenum
	_map(STORAGE storage)
	{
		switch (storage)
		{
		case STORAGE::STATIC:
			return GL_STATIC_DRAW;
			break;
		case STORAGE::DYNAMIC:
			return GL_DYNAMIC_DRAW;
			break;
		default:
			break;
		}
	}

	GLuint
	_shader_obj(std::ifstream& stream, const char* shader_path, SHADER_STAGE shader_stage)
	{
		stream.open(shader_path);
		if (!stream.is_open())
			assert("shader file not found" && false);

		std::string str;
		while (!stream.eof()) {
			std::string line;
			std::getline(stream, line);
			str += line + "\n";
		}
		stream.close();

		GLchar* src[1];
		GLint length[1];
		src[0] = &str.front();
		length[0] = (GLint)str.size();
		GLuint obj = glCreateShader(_map(shader_stage));
		glShaderSource(obj, 1, src, length);
		glCompileShader(obj);

		//check if shader compiled
		int success;
		char infoLog[512];
		glGetShaderiv(obj, GL_COMPILE_STATUS, &success);
		if (success == false)
		{
			glGetShaderInfoLog(obj, 512, nullptr, infoLog);
			printf(infoLog);
			assert("ERROR: shader couldn't be compiled." && false);
		}
		return obj;
	}

	Program
	program_create(const char* vertex_shader_path, const char* pixel_shader_path)
	{
		IGL_Handle* handle = new IGL_Handle{};
		handle->kind = IGL_Handle::KIND::KIND_PROGRAM;
		handle->program.id = glCreateProgram();
		std::ifstream stream;
		GLuint vobj = _shader_obj(stream, vertex_shader_path, SHADER_STAGE::VERTEX);
		GLuint pobj = _shader_obj(stream, pixel_shader_path, SHADER_STAGE::PIXEL);
		glAttachShader(handle->program.id, vobj);
		glAttachShader(handle->program.id, pobj);
		glLinkProgram(handle->program.id);
		glDeleteShader(vobj);
		glDeleteShader(pobj);
		error();
		return handle;
	}

	Program
	program_create(const char* vertex_shader_path, const char* geometry_shader_path, const char* pixel_shader_path)
	{
		IGL_Handle* handle = new IGL_Handle{};
		handle->kind = IGL_Handle::KIND::KIND_PROGRAM;
		handle->program.id = glCreateProgram();
		std::ifstream stream;
		GLuint vobj = _shader_obj(stream, vertex_shader_path, SHADER_STAGE::VERTEX);
		GLuint gobj = _shader_obj(stream, geometry_shader_path, SHADER_STAGE::GEOMETRY);
		GLuint pobj = _shader_obj(stream, pixel_shader_path, SHADER_STAGE::PIXEL);
		glAttachShader(handle->program.id, vobj);
		glAttachShader(handle->program.id, gobj);
		glAttachShader(handle->program.id, pobj);
		glLinkProgram(handle->program.id);
		glDeleteShader(vobj);
		glDeleteShader(gobj);
		glDeleteShader(pobj);
		error();
		return handle;
	}

	void
	program_use(Program prog)
	{
		glUseProgram(prog->program.id);
	}

	void
	program_delete(Program prog)
	{
		glDeleteProgram(prog->program.id);
		delete prog;
	}

	Buffer
	buffer_vertex_create()
	{
		IGL_Handle* self = new IGL_Handle{};
		self->kind = IGL_Handle::KIND::KIND_BUFFER;
		self->buffer.type = GL_ARRAY_BUFFER;
		glGenBuffers(1, &self->buffer.id);
		return self;
	}

	void
	buffer_vertex_set(Buffer self, const void* arr, int size_bytes, STORAGE storage)
	{
		glBindBuffer(GL_ARRAY_BUFFER, self->buffer.id);
		glBufferData(GL_ARRAY_BUFFER, size_bytes, arr, _map(storage));
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}

	void
	buffer_vertex_attribute(Buffer self, int att, int att_components_number, int size_bytes, int offset)
	{
		glBindBuffer(GL_ARRAY_BUFFER, self->buffer.id);
		glEnableVertexAttribArray(att);
		glVertexAttribPointer(att, att_components_number, GL_FLOAT, GL_FALSE, size_bytes, (void*)offset);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}

	void
	buffer_vertex_bind(Buffer self)
	{
		glBindBuffer(GL_ARRAY_BUFFER, self->buffer.id);
	}

	void
	buffer_vertex_unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}

	Buffer
	buffer_index_create(unsigned int indices[], std::size_t count)
	{
		IGL_Handle* self = new IGL_Handle{};
		self->kind = IGL_Handle::KIND::KIND_BUFFER;
		self->buffer.type = GL_ELEMENT_ARRAY_BUFFER;

		glGenBuffers(1, &self->buffer.id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->buffer.id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
		return self;
	}

	Buffer
	buffer_uniform_create(unsigned int size_in_bytes)
	{
		IGL_Handle* self = new IGL_Handle{};
		self->kind = IGL_Handle::KIND::KIND_BUFFER;
		self->buffer.type = GL_UNIFORM_BUFFER;

		glGenBuffers(1, &self->buffer.id);
		glBindBuffer(GL_UNIFORM_BUFFER, self->buffer.id);
		glBufferData(GL_UNIFORM_BUFFER, size_in_bytes, NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, NULL);
		return self;
	}

	void
	buffer_uniform_bind(unsigned int binding_point, Buffer data)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, data->buffer.id);
	}

	void
	buffer_uniform_set(Buffer buf, void* data, unsigned int size_in_bytes)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, buf->buffer.id);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, size_in_bytes, data);
		glBindBuffer(GL_UNIFORM_BUFFER, NULL);
	}

	void
	buffer_delete(Buffer buf)
	{
		glDeleteBuffers(1, &buf->buffer.id);
		delete buf;
	}

	Vao
	vao_create()
	{
		IGL_Handle* self = new IGL_Handle{};
		self->kind = IGL_Handle::KIND::KIND_VAO;
		glGenVertexArrays(1, &self->vao.id);
		return self;
	}

	void
	vao_bind(Vao va)
	{
		glBindVertexArray(va->vao.id);
	}

	void
	vao_unbind()
	{
		glBindVertexArray(NULL);
	}

	void
	vao_attach(Vao vao, Buffer vbo)
	{
		vao_bind(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo->buffer.id);
	}

	void
	vao_attach(Vao vao, Buffer vbo, Buffer ibo)
	{
		vao_bind(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo->buffer.id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->buffer.id);
	}

	void
	vao_delete(Vao va)
	{
		glDeleteVertexArrays(1, &va->vao.id);
	}

	Texture
	texture2d_create(vec2f size, INTERNAL_TEXTURE_FORMAT internal_format, EXTERNAL_TEXTURE_FORMAT format, DATA_TYPE type, bool mipmap)
	{
		IGL_Handle* handle = new IGL_Handle{};
		handle->kind = IGL_Handle::KIND::KIND_TEXTURE;

		handle->texture.width = size[0];
		handle->texture.height = size[1];
		handle->texture.internal_format = _map(internal_format);
		handle->texture.pixel_format = _map(format);
		handle->texture.type = _map(type);

		glGenTextures(1, &handle->texture.id);
		glBindTexture(GL_TEXTURE_2D, handle->texture.id);
		glTexImage2D(GL_TEXTURE_2D, 0, handle->texture.internal_format, handle->texture.width, handle->texture.height, 0, handle->texture.pixel_format, handle->texture.type,NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (mipmap)
			glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, NULL);

		return handle;
	}

	Texture
	texture2d_create(const io::Image& img, IMAGE_FORMAT format)
	{
		IGL_Handle* handle = new IGL_Handle{};
		handle->kind = IGL_Handle::KIND::KIND_TEXTURE;

		INTERNAL_TEXTURE_FORMAT internal_format;
		EXTERNAL_TEXTURE_FORMAT tex_format;
		DATA_TYPE type;
		switch (format)
		{
		case IMAGE_FORMAT::BMP:
		case IMAGE_FORMAT::PNG:
		case IMAGE_FORMAT::JPG:
			internal_format = INTERNAL_TEXTURE_FORMAT::RGB;
			tex_format = EXTERNAL_TEXTURE_FORMAT::RGB;
			type = DATA_TYPE::UBYTE;
			break;
		case IMAGE_FORMAT::HDR:
			internal_format = INTERNAL_TEXTURE_FORMAT::RGB16F;
			tex_format = EXTERNAL_TEXTURE_FORMAT::RGB;
			type = DATA_TYPE::FLOAT;
			break;
		default:
			assert("unsuported image format" && false);
			break;
		}

		handle->texture.width = img.width;
		handle->texture.height = img.height;
		handle->texture.internal_format = _map(internal_format);
		handle->texture.pixel_format = _map(tex_format);
		handle->texture.type = _map(type);

		glGenTextures(1, &handle->texture.id);
		glBindTexture(GL_TEXTURE_2D, handle->texture.id);
		// revisit -- glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, handle->texture.internal_format, handle->texture.width, handle->texture.height, 0, handle->texture.pixel_format, handle->texture.type, img.data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, NULL);

		return handle;
	}

	Texture
	texture2d_create(const char* image_path, IMAGE_FORMAT format)
	{
		Image img = image_read(image_path, format);
		Texture tex = texture2d_create(img, format);
		image_free(img);
		return tex;
	}

	void
	texture2d_resize(Texture tex, math::vec2f size, INTERNAL_TEXTURE_FORMAT internal_format, EXTERNAL_TEXTURE_FORMAT format, DATA_TYPE type)
	{
		tex->texture.width = size[0];
		tex->texture.height = size[1];
		tex->texture.internal_format = _map(internal_format);
		tex->texture.pixel_format = _map(format);
		tex->texture.type = _map(type);

		glDeleteTextures(1, &tex->texture.id);
		glGenTextures(1, &tex->texture.id);
		glBindTexture(GL_TEXTURE_2D, tex->texture.id);
		glTexImage2D(GL_TEXTURE_2D, 0, tex->texture.internal_format, tex->texture.width, tex->texture.height, 0, tex->texture.pixel_format, tex->texture.type, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, NULL);
	}

	math::vec2f
	texture2d_size(Texture tex)
	{
		return math::vec2f{ (float)tex->texture.width, (float)tex->texture.height };
	}

	void
	texture2d_render_offline_to(Texture output, Program prog, vec2f view_size)
	{
		GLuint fbo;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (GLuint)output->texture.id, 0);

		//setup
		program_use(prog);
		glViewport(0, 0, view_size[0], view_size[1]);

		//render to output attached texture
		Buffer quad_vbo = buffer_vertex_create();
		Vao quad_vao = vao_create();
		vao_attach(quad_vao, quad_vbo);
		buffer_vertex_set(quad_vbo, quad_ndc,   sizeof(quad_ndc), STORAGE::STATIC);
		buffer_vertex_attribute(quad_vbo, 0, 3, sizeof(world::Vertex), 0);
		buffer_vertex_attribute(quad_vbo, 1, 3, sizeof(world::Vertex), 3 * sizeof(float));
		buffer_vertex_attribute(quad_vbo, 2, 2, sizeof(world::Vertex), 6 * sizeof(float));
		vao_unbind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		vao_bind(quad_vao);
		draw_strips(6);
		vao_unbind();
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);

		glDeleteFramebuffers(1, &fbo);
		vao_delete(quad_vao);
		buffer_delete(quad_vbo);
	}

	void
	texture2d_bind(Texture texture, unsigned int texture_unit_index)
	{
		glActiveTexture(GL_TEXTURE0 + texture_unit_index);
		glBindTexture(GL_TEXTURE_2D, (GLuint)texture->texture.id);
	}

	void
	texture2d_unbind()
	{
		glBindTexture(GL_TEXTURE_2D, NULL);
	}

	void
	texture2d_unpack(Texture texture, io::Image& image, EXTERNAL_TEXTURE_FORMAT format, DATA_TYPE type)
	{
		texture2d_bind(texture, 0);
		glGetTexImage(GL_TEXTURE_2D, 0, texture->texture.pixel_format, texture->texture.type, image.data);
	}

	void
	texture_free(Texture texture)
	{
		glDeleteTextures(1, &texture->texture.id);
		delete texture;
	}

	Sampler
	sampler_create(TEXTURE_FILTERING min_filtering, TEXTURE_FILTERING mag_filtering, TEXTURE_SAMPLING sampling)
	{
		//go for handles pool?
		IGL_Handle* handle = new IGL_Handle;
		handle->kind = IGL_Handle::KIND::KIND_SAMPLER;

		GLuint* id = (GLuint*)&handle->sampler.id;
		handle->sampler.min_filtering = _map(min_filtering);
		handle->sampler.mag_filtering = _map(mag_filtering);
		handle->sampler.sampling = _map(sampling);

		glGenSamplers(1, id);
		glSamplerParameteri(*id, GL_TEXTURE_MIN_FILTER, handle->sampler.min_filtering);
		glSamplerParameteri(*id, GL_TEXTURE_MAG_FILTER, handle->sampler.mag_filtering);
		glSamplerParameteri(*id, GL_TEXTURE_WRAP_S, handle->sampler.sampling);
		glSamplerParameteri(*id, GL_TEXTURE_WRAP_T, handle->sampler.sampling);
		glSamplerParameteri(*id, GL_TEXTURE_WRAP_R, handle->sampler.sampling);

		return handle;
	}

	void
	sampler_bind(Sampler self, unsigned int texture_unit)
	{
		glBindSampler(texture_unit, self->sampler.id);
	}

	void
	sampler_free(Sampler self)
	{
		glDeleteSamplers(1, (GLuint*)&self->sampler.id);
		delete self;
	}

	Cubemap
	cubemap_create(vec2f view_size, INTERNAL_TEXTURE_FORMAT internal_format, EXTERNAL_TEXTURE_FORMAT pixel_format, DATA_TYPE type, bool mipmap)
	{
		IGL_Handle* handle = new IGL_Handle;
		handle->kind = IGL_Handle::KIND::KIND_CUBEMAP;
		handle->cubemap.width = view_size[0];
		handle->cubemap.height = view_size[1];
		handle->cubemap.internal_format = _map(internal_format);
		handle->cubemap.pixel_format = _map(pixel_format);
		handle->cubemap.type = _map(type);

		glGenTextures(1, &handle->cubemap.id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, handle->cubemap.id);
		for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, handle->cubemap.internal_format, view_size[0], view_size[1], 0, handle->cubemap.pixel_format, handle->cubemap.type, NULL);

		if (mipmap)
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		glBindTexture(GL_TEXTURE_CUBE_MAP, NULL);
		return handle;
	}

	Cubemap
	cubemap_rgba_create(const io::Image imgs[6])
	{
		IGL_Handle* handle = new IGL_Handle;
		handle->kind = IGL_Handle::KIND::KIND_CUBEMAP;
		handle->cubemap.width = imgs[0].width;
		handle->cubemap.height = imgs[0].height;
		handle->cubemap.internal_format = GL_RGB;
		handle->cubemap.pixel_format = GL_RGB;
		handle->cubemap.type = GL_UNSIGNED_BYTE;
		
		glGenTextures(1, &handle->cubemap.id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, handle->cubemap.id);

		//righ, left, top, bottom, front, back
		for (int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, imgs[i].width, imgs[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgs[i].data);

		glBindTexture(GL_TEXTURE_CUBE_MAP, NULL);

		return handle;
	}

	Cubemap
	cubemap_hdr_create(const io::Image& img, vec2f view_size, bool mipmap)
	{
		//create hdr texture
		Texture hdr = texture2d_create(img, IMAGE_FORMAT::HDR);
		Sampler sampler = sampler_create(TEXTURE_FILTERING::LINEAR, TEXTURE_FILTERING::LINEAR, TEXTURE_SAMPLING::CLAMP_TO_EDGE);

		//convert HDR equirectangular environment map to cubemap
		//create 6 views that will be rendered to the cubemap using equarectangular shader
		//don't use ortho projection as this will make z in NDC the same so your captures will look like duplicated
		//1.00000004321 is tan(45 degrees)
		Mat4f proj = proj_prespective_matrix(100, 0.1, 1, -1, 1, -1, 1.00000004321);
		Mat4f views[6] =
		{
			view_lookat_matrix(vec3f{-0.001f,  0.0f,  0.0f}, vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f, -1.0f,  0.0f}),
			view_lookat_matrix(vec3f{0.001f,  0.0f,  0.0f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f, -1.0f,  0.0f}),
			view_lookat_matrix(vec3f{0.0f, -0.001f,  0.0f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f,  0.0f,  1.0f}),
			view_lookat_matrix(vec3f{0.0f,  0.001f,  0.0f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f,  0.0f,  -1.0f}),
			view_lookat_matrix(vec3f{0.0f,  0.0f, -0.001f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f, -1.0f,  0.0f}),
			view_lookat_matrix(vec3f{0.0f,  0.0f,  0.001f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f, -1.0f,  0.0f})
		};

		//create env cubemap
		//(HDR should a 32 bit for each channel to cover a wide range of colors,
		//they make the exponent the alpha and each channel remains 8 so 16 bit for each -RGB-)
		Cubemap cube_map = cubemap_create(view_size, INTERNAL_TEXTURE_FORMAT::RGB16F, EXTERNAL_TEXTURE_FORMAT::RGB, DATA_TYPE::FLOAT, mipmap);

		//framebuffer to render to
		GLuint fbo;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		//setup
		Program prog = program_create(DIR_PATH"/engine/shaders/cube.vertex", DIR_PATH"/engine/shaders/equarectangular_to_cubemap.pixel");
		program_use(prog);
		texture2d_bind(hdr, 0);
		sampler_bind(sampler, 0);
		error();

		//render offline to the output cubemap texs
		glViewport(0, 0, view_size[0], view_size[1]);
		Buffer cube_vbo = buffer_vertex_create();
		Vao cube_vao = vao_create();
		vao_attach(cube_vao, cube_vbo);
		buffer_vertex_set(cube_vbo, unit_cube, sizeof(unit_cube), STORAGE::STATIC);
		buffer_vertex_attribute(cube_vbo, 0, 3, sizeof(world::Vertex), 0);
		buffer_vertex_attribute(cube_vbo, 1, 3, sizeof(world::Vertex), 3 * sizeof(float));
		buffer_vertex_attribute(cube_vbo, 2, 2, sizeof(world::Vertex), 6 * sizeof(float));
		vao_unbind();

		error();
		for (unsigned int i = 0; i < 6; ++i)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cube_map->cubemap.id, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			uniformmat4f_set(prog, "vp", proj * views[i]);
			vao_bind(cube_vao);
			draw_strips(36);
			vao_unbind();
		}

		if (mipmap)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map->cubemap.id);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, NULL);
		}

		texture2d_unbind();
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);

		//free
		glDeleteFramebuffers(1, &fbo);
		vao_delete(cube_vao);
		buffer_delete(cube_vbo);
		program_delete(prog);
		texture_free(hdr);
		sampler_free(sampler);

		return cube_map;
	}

	void
	cubemap_postprocess(Cubemap input, Cubemap output, Program postprocessor, Unifrom_Float uniform, vec2f view_size, int mipmap_level)
	{
		//convert HDR equirectangular environment map to cubemap
		//create 6 views that will be rendered to the cubemap using equarectangular shader
		//1.00000004321 is tan(45 degrees)
		Mat4f proj = proj_prespective_matrix(100, 0.1, 1, -1, 1, -1, 1.00000004321);
		Mat4f views[6] =
		{
			view_lookat_matrix(vec3f{-0.001f,  0.0f,  0.0f}, vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f, -1.0f,  0.0f}),
			view_lookat_matrix(vec3f{0.001f,  0.0f,  0.0f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f, -1.0f,  0.0f}),
			view_lookat_matrix(vec3f{0.0f, -0.001f,  0.0f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f,  0.0f,  1.0f}),
			view_lookat_matrix(vec3f{0.0f,  0.001f,  0.0f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f,  0.0f,  -1.0f}),
			view_lookat_matrix(vec3f{0.0f,  0.0f, -0.001f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f, -1.0f,  0.0f}),
			view_lookat_matrix(vec3f{0.0f,  0.0f,  0.001f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f, -1.0f,  0.0f})
		};


		GLuint fbo;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		//convolute
		Sampler sampler = sampler_create(TEXTURE_FILTERING::LINEAR, TEXTURE_FILTERING::LINEAR, TEXTURE_SAMPLING::CLAMP_TO_EDGE);
		program_use(postprocessor);
		cubemap_bind(input, 0);
		sampler_bind(sampler, 0);

		//assign float uniforms (move to arrays)
		uniform1f_set(postprocessor, uniform.uniform, uniform.value);

		//render offline to the output cubemap texs
		glViewport(0, 0, view_size[0], view_size[1]);
		Buffer cube_vbo = buffer_vertex_create();
		Vao cube_vao = vao_create();
		vao_attach(cube_vao, cube_vbo);
		buffer_vertex_set(cube_vbo, unit_cube,  sizeof(unit_cube), STORAGE::STATIC);
		buffer_vertex_attribute(cube_vbo, 0, 3, sizeof(world::Vertex), 0);
		buffer_vertex_attribute(cube_vbo, 1, 3, sizeof(world::Vertex), 3 * sizeof(float));
		buffer_vertex_attribute(cube_vbo, 2, 2, sizeof(world::Vertex), 6 * sizeof(float));
		vao_unbind();

		//TEST
		/*io::Image imgs[6];
		for (int i = 0; i < 6; ++i)
			imgs[i] = io::image_new(4, view_size)*/

		for (unsigned int i = 0; i < 6; ++i)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, output->cubemap.id, mipmap_level);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			uniformmat4f_set(postprocessor, "vp", proj * views[i]);
			vao_bind(cube_vao);
			draw_strips(36);
			vao_unbind();

			//TEST
			/*glReadPixels(0, 0, view_size[0], view_size[1], GL_RGBA, GL_UNSIGNED_BYTE, imgs[i].data);
			std::string name = "../res/imgs/specular_test/img_" + std::to_string(i) + std::to_string(mipmap_level) +".png";
			io::image_write(imgs[i], name.c_str(), io::IMAGE_FORMAT::PNG);*/
		}

		//TEST - free
		/*for (int i = 0; i < 6; ++i)
			io::image_free(imgs[i]);*/

		texture2d_unbind();
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);

		//free
		glDeleteFramebuffers(1, &fbo);
		vao_delete(cube_vao);
		buffer_delete(cube_vbo);
		sampler_free(sampler);
	}

	void
	cubemap_bind(Cubemap cmap, unsigned int texture_unit_index)
	{
		glActiveTexture(GL_TEXTURE0 + texture_unit_index);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cmap->cubemap.id);
	}

	void
	cubemap_free(Cubemap cmap)
	{
		glDeleteTextures(1, &cmap->cubemap.id);
		delete cmap;
	}

	Framebuffer
	framebuffer_create()
	{
		IGL_Handle* self = new IGL_Handle{};
		self->kind = IGL_Handle::KIND::KIND_FRAMEBUFFER;
		glGenFramebuffers(1, &self->framebuffer.id);

		return self;
	}

	void
	framebuffer_bind(Framebuffer fb)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fb->framebuffer.id);
	}

	void
	framebuffer_attach(Framebuffer fb, Texture tex, FRAMEBUFFER_ATTACHMENT attachment)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fb->framebuffer.id);
		glFramebufferTexture2D(GL_FRAMEBUFFER, _map(attachment), GL_TEXTURE_2D, (GLuint)tex->texture.id, 0);
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "Error while creating framebuffer");
	}

	void
	framebuffer_unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	}

	void
	framebuffer_free(Framebuffer fb)
	{
		glDeleteFramebuffers(1, &fb->framebuffer.id);
	}

	void
	disable_color_buffer_rw()
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	void
	depth_clear()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void
	frame_start(float r, float g, float b)
	{
		glClearColor(r, g, b, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthRange(0, 1);
		glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}

	void
	color_clear(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
	}

	void
	depth_test(DEPTH_TEST test)
	{
		glDepthFunc(_map(test));
	}

	void
	poly_edges_enable()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void
	poly_edges_disable()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void
	draw_points(std::size_t vertices_count, float thickness)
	{
		glPointSize(thickness);
		glDrawArrays(GL_POINTS, 0, (GLsizei)vertices_count);
	}

	void
	draw_lines(std::size_t vertices_count, float thickness)
	{
		glLineWidth(thickness);
		glDrawArrays(GL_LINES, 0, (GLsizei)vertices_count);
	}

	void
	draw_strips(std::size_t vertices_count)
	{
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices_count);
	}

	void
	draw_indexed(unsigned int indcies_count)
	{
		glDrawElements(GL_TRIANGLES, indcies_count, GL_UNSIGNED_INT, 0);
	}

	void
	uniform1f_set(Program prog, const char* uniform, float data)
	{
		int uniform_loc = glGetUniformLocation((GLuint)prog->program.id, uniform);
		glUniform1f(uniform_loc, data);
	}

	void
	uniform3f_set(Program prog, const char * uniform, const math::vec3f & data)
	{
		int uniform_loc = glGetUniformLocation((GLuint)prog->program.id, uniform);
		glUniform3f(uniform_loc, data[0], data[1], data[2]);
	}

	void
	uniform4f_set(Program prog, const char* uniform, const math::vec4f& data)
	{
		int uniform_loc = glGetUniformLocation((GLuint)prog->program.id, uniform);
		glUniform4f(uniform_loc, data[0], data[1], data[2], data[3]);
	}

	void
	uniformmat4f_set(Program prog, const char* uniform, const math::Mat4f& data)
	{
		int uniform_loc = glGetUniformLocation((GLuint)prog->program.id, uniform);
		glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, &data.data[0][0]);
	}

	void
	view_port(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	bool
	error()
	{
		GLenum err = glGetError();
		switch (err)
		{
			case GL_INVALID_ENUM:
				assert(false && "invalid enum value was passed");
				return false;

			case GL_INVALID_VALUE:
				assert(false && "invalid value was passed");
				return false;

			case GL_INVALID_OPERATION:
				assert(false && "invalid operation at the current state of opengl");
				return false;

			case GL_INVALID_FRAMEBUFFER_OPERATION:
				assert(false && "invalid framebuffer operation");
				return false;

			case GL_OUT_OF_MEMORY:
				assert(false && "out of memory");
				return false;

			case GL_STACK_UNDERFLOW:
				assert(false && "stack underflow");
				return false;

			case GL_STACK_OVERFLOW:
				assert(false && "stack overflow");
				return false;

			case GL_NO_ERROR:
			default:
				return true;
		}
	}
};