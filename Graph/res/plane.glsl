#shader vertex
#version 450 core
layout(location = 0) in vec2 a_Pos;

uniform mat4 u_VP;

void main()
{
	gl_Position =  u_VP * vec4(a_Pos, 0.0f, 1.0f);
}

#shader fragment
#version 450 core
out vec4 FragColor;

uniform vec3 u_Color;

void main()
{
	FragColor = vec4(u_Color, 1.0f);
}