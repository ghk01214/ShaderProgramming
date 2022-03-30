#version 450

in vec3 a_Position;
uniform float uTime;

void main()
{
	gl_Position = vec4(a_Position * uTime, 1);
}
