#version 450

in vec3 position;
in vec2 inTexPos;

out vec2 outTexPos;

void main()
{
	gl_Position = vec4(position, 1);
	outTexPos = inTexPos;
}
