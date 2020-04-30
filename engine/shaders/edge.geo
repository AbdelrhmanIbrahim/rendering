//NOT USED in edge renderer, replaced by glPolygonMode enable

#version 400 core
#extension GL_ARB_shading_language_420pack : require

layout(triangles) in;
layout(line_strip, max_vertices = 4) out;

layout (std140, binding = 1) uniform Cull
{
	int backface_cull;
};

void
emit_edges()
{
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	EndPrimitive();
}

void
main() 
{
	if(backface_cull != 0)
	{
		vec3 e0 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
		vec3 e1 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
		vec3 normal = normalize(cross(e0, e1));
		if(dot(normal, vec3(0,0,-1)) < 0)
			emit_edges();
	}
	else
		emit_edges();
}
