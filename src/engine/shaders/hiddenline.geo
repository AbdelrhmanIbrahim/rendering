#version 400 core
#extension GL_ARB_shading_language_420pack : require

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec3 barycentric;

void
main()
{
	barycentric = vec3(1,0,0);
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	barycentric = vec3(0,1,0);
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	barycentric = vec3(0,0,1);
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
}
