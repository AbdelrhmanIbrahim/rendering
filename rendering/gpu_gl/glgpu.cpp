#include "gpu_gl/glgpu.h"

#include <glew.h>

#include <assert.h>

#include <fstream>
#include <string>

#include "IO/Image.h"

using namespace io;

namespace glgpu
{
	void
	graphics_init()
	{

		GLenum gl_ok = glewInit();
		assert(gl_ok == GLEW_OK);
	}

	enum SHADER_STAGE
	{
		VERTEX,
		PIXEL
	};

	int
	_map(TEXTURE_UNIT unit)
	{
		switch (unit)
		{
		case TEXTURE_UNIT::UNIT_0:
			return GL_TEXTURE0;
		default:
			assert("undefined texture unit" && false);
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
		length[0] = str.size();
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
	vertex_buffer_create(geo::Vertex vertices[], unsigned int count)
	{
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(geo::Vertex), vertices, GL_STATIC_DRAW);
		return (buffer)vbo;
	}

	buffer
	index_buffer_create(unsigned int indices[], unsigned int count)
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
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(geo::Vertex), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(geo::Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(geo::Vertex), (void*)(6 * sizeof(float)));
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
	texture_create(const char* image_path)
	{
		Image img = image_read(image_path);

		GLuint tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// revisit -- glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);
		glBindTexture(GL_TEXTURE_2D, NULL);
		image_free(img);

		return (texture)tex;
	}

	void
	texture_bind(texture texture, TEXTURE_UNIT texture_unit)
	{
		glActiveTexture(_map(texture_unit));
		glBindTexture(GL_TEXTURE_2D, (GLuint)texture);
	}

	void
	texture_free(texture texture)
	{
		GLuint t = (GLuint)texture;
		glDeleteTextures(1, &t);
	}

	void
	color_clear(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_DEPTH);
	}

	void
	draw_indexed(unsigned int indcies_count)
	{
		glDrawElements(GL_TRIANGLES, indcies_count, GL_UNSIGNED_INT, 0);
	}

	void
	uniform4f_set(program prog, const char* uniform, math::vec4f& data)
	{
		int uniform_loc = glGetUniformLocation((GLuint)prog, uniform);
		glUniform4f(uniform_loc, data[0], data[1], data[2], data[3]);
	}

	void
	uniform1i_set(program prog, const char* uniform, int data)
	{
		int uniform_loc = glGetUniformLocation((GLuint)prog, uniform);
		glUniform1i(uniform_loc, data);
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