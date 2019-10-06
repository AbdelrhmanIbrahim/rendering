#include "gpu_gl/glgpu.h"

#include <glew.h>

#include <assert.h>

namespace glgpu
{
	void
	graphics_init()
	{
		GLenum gl_ok = glewInit();
		assert(gl_ok == GLEW_OK);
	}

	program
	program_create(const char* vertex_shader, const char* pixel_shader)
	{
		int  success;
		char infoLog[512];

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertex_shader, nullptr);
		glCompileShader(vs);

		//check if vertex shader compiled
		glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
		if (success == false)
		{
			glGetShaderInfoLog(vs, 512, nullptr, infoLog);
			assert("ERROR: vertex shader couldn't be compiled." && false);
		}

		GLuint ps =  glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(ps, 1, &pixel_shader, nullptr);
		glCompileShader(ps);

		//check if pixel shader compiled
		glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
		if (success == false)
		{
			glGetShaderInfoLog(ps, 512, nullptr, infoLog);
			assert("ERROR: pixel shader couldn't be compiled." && false);
		}

		GLuint prog =  glCreateProgram();
		glAttachShader(prog, vs);
		glAttachShader(prog, ps);
		glLinkProgram(prog);
		glDeleteShader(vs);
		glDeleteShader(ps);

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
	vertex_buffer_create(geo::Vertex vertices[])
	{
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(geo::Vertex), vertices, GL_STATIC_DRAW);
		return (buffer)vbo;
	}

	buffer
	index_buffer_create(unsigned int indices[])
	{
		GLuint ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
		return (buffer)ebo;
	}

	buffer
	buffer_create()
	{
		GLuint buf;
		glGenBuffers(1, &buf);
		return (buffer)buf;
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

	void
	color_clear(float r, float g, float b)
	{
		glClearColor(r, g, b, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void
	draw_indexed(unsigned int indcies_count)
	{
		glDrawElements(GL_TRIANGLES, indcies_count, GL_UNSIGNED_INT, 0);
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