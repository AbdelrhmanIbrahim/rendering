#include "gpu_gl/glgpu.h"

#include <glew.h>
#include <assert.h>
#include <fstream>
#include <string>
#include <iostream>

#include <math/Matrix.h>

#include <math/Gfx.h>

using namespace io;
using namespace math;
using namespace geo;

namespace glgpu
{
	//unit cube can be used in some algorithms like equirectangle mapping, irradiance map, etc..
	constexpr static Vertex unit_cube[36] =
	{
		//front
		Vertex{-1.0f, -1.0f, 1.0},
		Vertex{1.0f, -1.0f, 1.0f},
		Vertex{-1.0f, 1.0f, 1.0f},
		Vertex{-1.0f, 1.0f, 1.0f},
		Vertex{1.0f, -1.0f, 1.0f},
		Vertex{1.0f, 1.0f, 1.0},

		//right
		Vertex{1.0f, -1.0f, 1.0f},
		Vertex{1.0f, 1.0f, -1.0f},
		Vertex{1.0f, 1.0f, 1.0f},
		Vertex{1.0f, 1.0f, -1.0f},
		Vertex{1.0f, -1.0f, 1.0f},
		Vertex{1.0f, -1.0f, -1.0},

		//bottom
		Vertex{1.0f, -1.0f, -1.0f},
		Vertex{1.0f, -1.0f, 1.0f},
		Vertex{-1.0f, -1.0f, -1.0f},
		Vertex{-1.0f, -1.0f, -1.0f},
		Vertex{1.0f, -1.0f, 1.0},
		Vertex{-1.0f, -1.0f, 1.0f},

		//left
		Vertex{-1.0f, -1.0f, -1.0f},
		Vertex{-1.0f, -1.0f, 1.0},
		Vertex{-1.0f, 1.0f, -1.0f},
		Vertex{-1.0f, 1.0f, -1.0f},
		Vertex{-1.0f, -1.0f, 1.0f,},
		Vertex{-1.0f, 1.0f, 1.0f},

		//top
		Vertex{-1.0f, 1.0f, -1.0f},
		Vertex{-1.0f, 1.0f, 1.0f},
		Vertex{1.0f, 1.0f, -1.0f},
		Vertex{1.0f, 1.0f, -1.0f},
		Vertex{-1.0f, 1.0f, 1.0f},
		Vertex{1.0f, 1.0f, 1.0f},

		//back
		Vertex{-1.0f, 1.0f, -1.0f},
		Vertex{1.0f, 1.0f, -1.0f,},
		Vertex{1.0f, -1.0f, -1.0f},
		Vertex{-1.0f, 1.0f, -1.0f,},
		Vertex{1.0f, -1.0f, -1.0f},
		Vertex{-1.0f, -1.0f, -1.0f}
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
		PIXEL
	};

	int
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

	int
	_map(TEXTURE_UNIT unit)
	{
		switch (unit)
		{
		case TEXTURE_UNIT::UNIT_0:
			return GL_TEXTURE0;
		case TEXTURE_UNIT::UNIT_1:
			return GL_TEXTURE1;

		default:
			assert("undefined texture unit" && false);
			return -1;
		}
	}

	int
	_map(SHADER_STAGE stage)
	{
		switch (stage)
		{
		case SHADER_STAGE::VERTEX:
			return GL_VERTEX_SHADER;
		case SHADER_STAGE::PIXEL:
			return GL_FRAGMENT_SHADER;
		default:
			assert("undefined shader stage" && false);
			return -1;
		}
	}

	int
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

	int
	_map(TEXTURE_FORMAT format)
	{
		switch (format)
		{
		case TEXTURE_FORMAT::RGB:
			return GL_RGB;
		case TEXTURE_FORMAT::RGBA:
			return GL_RGBA;
		case TEXTURE_FORMAT::DEPTH_STENCIL:
			return GL_DEPTH_STENCIL;
		default:
			assert("undefined format" && false);
			return -1;
		}
	}

	int
	_map(INTERNAL_TEXTURE_FORMAT format)
	{
		switch (format)
		{
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

	int
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

	program
	program_create(const char* vertex_shader_path, const char* pixel_shader_path)
	{
		std::ifstream stream;
		GLuint vobj = _shader_obj(stream, vertex_shader_path, SHADER_STAGE::VERTEX);
		GLuint pobj = _shader_obj(stream, pixel_shader_path, SHADER_STAGE::PIXEL);
		GLuint prog = glCreateProgram();
		glAttachShader(prog, vobj);
		glAttachShader(prog, pobj);
		glLinkProgram(prog);
		glDeleteShader(vobj);
		glDeleteShader(pobj);

		return (program)prog;
	}

	void
	program_use(program prog)
	{
		GLuint p = (GLuint)prog;
		glUseProgram(p);
	}

	void
	program_delete(program prog)
	{
		GLuint p = (GLuint)prog;
		glDeleteProgram(p);
	}

	buffer
	vertex_buffer_create(const geo::Vertex vertices[], std::size_t count)
	{
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(geo::Vertex), vertices, GL_STATIC_DRAW);
		return (buffer)vbo;
	}

	buffer
	index_buffer_create(unsigned int indices[], std::size_t count)
	{
		GLuint ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
		return (buffer)ebo;
	}

	void
	buffer_delete(buffer buf)
	{
		GLuint b = (GLuint)buf;
		glDeleteBuffers(1, &b);
	}

	vao
	vao_create()
	{
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		return vao(VAO);
	}

	void
	vao_bind(vao va, buffer vbo, buffer ebo)
	{
		GLuint v = (GLuint)va;
		glBindVertexArray(v);
		glBindBuffer(GL_ARRAY_BUFFER, (GLuint)vbo);

		//pos
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(geo::Vertex), (void*)0);

		//normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(geo::Vertex), (void*)(3 * sizeof(float)));

		//uv
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(geo::Vertex), (void*)(6 * sizeof(float)));

		//no indexed triangles so far
		if(ebo != NULL)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)ebo);
	}

	void
	vao_unbind()
	{
		glBindVertexArray(NULL);
	}

	void
	vao_delete(vao va)
	{
		GLuint v = (GLuint)va;
		glDeleteVertexArrays(1, &v);
	}

	texture
	cubemap_rgba_create(const io::Image imgs[6])
	{
		GLuint cubemap;
		glGenTextures(1, &cubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

		//righ, left, top, bottom, front, back
		for (int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, imgs[i].width, imgs[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgs[i].data);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, NULL);

		return (texture)cubemap;
	}

	texture
	cubemap_hdr_create(const io::Image& img)
	{
		//create hdr texture
		texture hdr = texture2d_create(img, IMAGE_FORMAT::HDR);

		//convert HDR equirectangular environment map to cubemap
		//create 6 views that will be rendered to the cubemap using equarectangular shader
		Mat4f proj = proj_ortho_matrix(100, 0.1, 1, -1, 1, -1);
		Mat4f views[6] =
		{
			view_lookat_matrix(vec3f{2.0f,  0.0f,  0.0f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f, -1.0f, 0.0f}), //X+
			view_lookat_matrix(vec3f{-2.0f,  0.0f,  0.0f}, vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f, -1.0f, 0.0f}), //X-
			view_lookat_matrix(vec3f{0.0f,  2.0f,  0.0f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f,  0.0f, -1.0f}), //Y+
			view_lookat_matrix(vec3f{0.0f, -2.0f,  0.0f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f,  0.0f, 1.0f}), //Y-
			view_lookat_matrix(vec3f{0.0f,  0.0f, -2.0f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f, -1.0f, 0.0f}), //Z-
			view_lookat_matrix(vec3f{0.0f,  0.0f,  2.0f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f, -1.0f, 0.0f}) //Z+
		};

		//create env cubemap
		//(HDR should a 32 bit for each channel to cover a wide range of colors,
		//they make the exponent the alpha and each channel remains 8 so 16 bit for each -RGB-)
		GLuint cube_map;
		vec2f view_size{ 800, 800 };
		glGenTextures(1, &cube_map);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map);
		for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, view_size[0], view_size[1], 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//float framebuffer to render to
		GLuint fbo, rbo;
		glGenFramebuffers(1, &fbo);
		glGenRenderbuffers(1, &rbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, view_size[0], view_size[1]);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

		glViewport(0, 0, view_size[0], view_size[1]);
		vao cube_vao = vao_create();
		buffer cube_vs = vertex_buffer_create(unit_cube, 36);
		program prog = program_create("../rendering/engine/shaders/cube.vertex", "../rendering/engine/shaders/equarectangular_to_cubemap.pixel");
		program_use(prog);
		texture2d_bind(hdr, TEXTURE_UNIT::UNIT_0);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cube_map, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			uniformmat4f_set(prog, "vp", proj * views[i]);
			vao_bind(cube_vao, cube_vs, NULL);
			draw_strip(36);
			vao_unbind();
		}
		texture2d_unbind();
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);

		//free
		glDeleteRenderbuffers(1, &rbo);
		glDeleteFramebuffers(1, &fbo);
		vao_delete(cube_vao);
		buffer_delete(cube_vs);
		program_delete(prog);
		texture_free(hdr);

		return (texture)cube_map;
	}

	texture
	cubemap_postprocess(texture cubemap, const char* pixel_shader)
	{
		GLuint cubemap_pp;

		//convert HDR equirectangular environment map to cubemap
		//create 6 views that will be rendered to the cubemap using equarectangular shader
		Mat4f proj = proj_ortho_matrix(100, 0.1, 1, -1, 1, -1);
		Mat4f views[6] =
		{
			view_lookat_matrix(vec3f{2.0f,  0.0f,  0.0f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f, -1.0f, 0.0f}), //X+
			view_lookat_matrix(vec3f{-2.0f,  0.0f,  0.0f}, vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f, -1.0f, 0.0f}), //X-
			view_lookat_matrix(vec3f{0.0f,  2.0f,  0.0f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f,  0.0f, -1.0f}), //Y+
			view_lookat_matrix(vec3f{0.0f, -2.0f,  0.0f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f,  0.0f, 1.0f}), //Y-
			view_lookat_matrix(vec3f{0.0f,  0.0f,  2.0f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f, -1.0f, 0.0f}), //Z+
			view_lookat_matrix(vec3f{0.0f,  0.0f, -2.0f},  vec3f{0.0f, 0.0f, 0.0f}, vec3f{0.0f, -1.0f, 0.0f}) //Z-
		};

		vec2f view_size{ 512, 512 };
		glGenTextures(1, &cubemap_pp);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_pp);
		for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, view_size[0], view_size[1], 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLuint fbo, rbo;
		glGenFramebuffers(1, &fbo);
		glGenRenderbuffers(1, &rbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, view_size[0], view_size[1]);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

		//convolute
		glViewport(0, 0, view_size[0], view_size[1]);
		vao cube_vao = vao_create();
		buffer cube_vs = vertex_buffer_create(unit_cube, 36);
		program prog = program_create("../rendering/engine/shaders/cube.vertex", "../rendering/engine/shaders/irradiance_convolution.pixel");
		program_use(prog);

		cubemap_bind(cubemap, TEXTURE_UNIT::UNIT_0);
		uniform1i_set(prog, "cubemap", TEXTURE_UNIT::UNIT_0);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap_pp, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			uniformmat4f_set(prog, "vp", proj * views[i]);
			vao_bind(cube_vao, cube_vs, NULL);
			draw_strip(36);
			vao_unbind();
		}
		texture2d_unbind();
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);

		//free
		glDeleteRenderbuffers(1, &rbo);
		glDeleteFramebuffers(1, &fbo);
		vao_delete(cube_vao);
		buffer_delete(cube_vs);
		program_delete(prog);

		return (texture)cubemap_pp;
	}

	void
	cubemap_bind(texture texture, TEXTURE_UNIT texture_unit)
	{
		glActiveTexture(_map(texture_unit));
		glBindTexture(GL_TEXTURE_CUBE_MAP, (GLuint)texture);
	}

	texture
	texture2d_create(int width, int height, INTERNAL_TEXTURE_FORMAT internal_format, TEXTURE_FORMAT format, DATA_TYPE type)
	{
		GLuint tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, _map(internal_format), width, height, 0, _map(format), _map(type), NULL);
		glBindTexture(GL_TEXTURE_2D, NULL);

		return (texture)tex;
	}

	texture
	texture2d_create(const io::Image& img, IMAGE_FORMAT format)
	{
		INTERNAL_TEXTURE_FORMAT internal_format;
		TEXTURE_FORMAT tex_format;
		DATA_TYPE type;
		switch (format)
		{
		case IMAGE_FORMAT::BMP:
		case IMAGE_FORMAT::PNG:
		case IMAGE_FORMAT::JPG:
			internal_format = INTERNAL_TEXTURE_FORMAT::RGB;
			tex_format = TEXTURE_FORMAT::RGB;
			type = DATA_TYPE::UBYTE;
			break;
		case IMAGE_FORMAT::HDR:
			internal_format = INTERNAL_TEXTURE_FORMAT::RGB16F;
			tex_format = TEXTURE_FORMAT::RGB;
			type = DATA_TYPE::FLOAT;
			break;
		default:
			assert("unsuported image format" && false);
			break;
		}

		GLuint tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// revisit -- glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, _map(internal_format), img.width, img.height, 0, _map(tex_format), _map(type), img.data);
		glBindTexture(GL_TEXTURE_2D, NULL);

		return (texture)tex;
	}

	texture
	texture2d_create(const char* image_path, IMAGE_FORMAT format)
	{
		Image img = image_read(image_path, format);
		texture tex = texture2d_create(img, format);
		image_free(img);
		return tex;
	}

	void
	texture2d_bind(texture texture, TEXTURE_UNIT texture_unit)
	{
		glActiveTexture(_map(texture_unit));
		glBindTexture(GL_TEXTURE_2D, (GLuint)texture);
	}

	void
	texture2d_unbind()
	{
		glBindTexture(GL_TEXTURE_2D, NULL);
	}

	void
	texture2d_unpack(texture texture, io::Image& image, TEXTURE_FORMAT format, DATA_TYPE type)
	{
		texture2d_bind(texture, TEXTURE_UNIT::UNIT_0);
		glGetTexImage(GL_TEXTURE_2D, 0, _map(format), _map(type), image.data);
	}

	void
	texture_free(texture texture)
	{
		GLuint t = (GLuint)texture;
		glDeleteTextures(1, &t);
	}

	framebuffer
	framebuffer_create()
	{
		unsigned int fb;
		glGenFramebuffers(1, &fb);

		return (framebuffer)fb;
	}

	void
	framebuffer_bind(framebuffer fb)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)fb);
	}

	void
	framebuffer_attach(framebuffer fb, texture tex, FRAMEBUFFER_ATTACHMENT attachment)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, _map(attachment), GL_TEXTURE_2D, (GLuint)tex, 0);
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "Error while creating framebuffer");
	}

	void
	framebuffer_unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	}

	void
	framebuffer_free(framebuffer fb)
	{
		GLuint t = (GLuint)fb;
		glDeleteFramebuffers(1, &t);
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
	frame_start()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LESS);
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
	draw_strip(std::size_t vertices_count)
	{
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices_count);
	}

	void
	draw_indexed(unsigned int indcies_count)
	{
		glDrawElements(GL_TRIANGLES, indcies_count, GL_UNSIGNED_INT, 0);
	}

	void
	uniform1f_set(program prog, const char* uniform, float data)
	{
		int uniform_loc = glGetUniformLocation((GLuint)prog, uniform);
		glUniform1f(uniform_loc, data);
	}

	void
	uniform3f_set(program prog, const char * uniform, const math::vec3f & data)
	{
		int uniform_loc = glGetUniformLocation((GLuint)prog, uniform);
		glUniform3f(uniform_loc, data[0], data[1], data[2]);
	}

	void
	uniform4f_set(program prog, const char* uniform, const math::vec4f& data)
	{
		int uniform_loc = glGetUniformLocation((GLuint)prog, uniform);
		glUniform4f(uniform_loc, data[0], data[1], data[2], data[3]);
	}

	void
	uniformmat4f_set(program prog, const char* uniform, const math::Mat4f& data)
	{
		int uniform_loc = glGetUniformLocation((GLuint)prog, uniform);
		glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, &data.data[0][0]);
	}

	void
	uniform1i_set(program prog, const char* uniform, int data)
	{
		//samplers for example
		int uniform_loc = glGetUniformLocation((GLuint)prog, uniform);
		glUniform1i(uniform_loc, data);
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