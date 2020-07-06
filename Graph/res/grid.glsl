#shader vertex
#version 450 core
layout(location = 0) in vec2 a_Pos;

uniform mat4 u_VP;

uniform float u_OffSet;

out mat4 MVP;
out float Offset;

void main()
{
	MVP = u_VP;
	Offset = u_OffSet;
	gl_Position = vec4(a_Pos, 0.0f, 1.0f);
}

#shader geometry
#version 450 core
layout (points) in;
layout (line_strip, max_vertices = 5) out;

in mat4 MVP[];
in float Offset[];

void main()
{
	float offset = Offset[0];

	gl_Position = MVP[0]*(gl_in[0].gl_Position + (vec4(-offset, offset, 0.f, 1.f))); //top left
	EmitVertex();
	gl_Position = MVP[0]*(gl_in[0].gl_Position + (vec4(offset, offset, 0.f, 1.f))); //top right
	EmitVertex();
	gl_Position = MVP[0]*(gl_in[0].gl_Position + (vec4(offset, -offset, 0.f, 1.f))); //bottom right
	EmitVertex();
	gl_Position = MVP[0]*(gl_in[0].gl_Position + (vec4(-offset, -offset, 0.f, 1.f))); //bottom left
	EmitVertex();
	gl_Position = MVP[0]*(gl_in[0].gl_Position + (vec4(-offset, offset, 0.f, 1.f))); //top left
	EmitVertex();

	EndPrimitive();
}

#shader fragment
#version 450 core
out vec4 FragColor;

uniform vec3 u_Color;

void main()
{
	FragColor = vec4(u_Color, 1.0f);
}