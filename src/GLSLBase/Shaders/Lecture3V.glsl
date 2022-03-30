#version 450

in vec3 a_Position;

in vec4 inColor;
out vec4 outColor;
uniform float uTime;

void main()
{
	gl_Position = vec4(a_Position * uTime, 1);

	outColor = inColor;
}
