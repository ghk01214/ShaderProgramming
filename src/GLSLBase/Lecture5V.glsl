#version 450

in vec3 position;
uniform float time;

const float PI = 3.141592;

void main()
{
	//vec4 pos = vec4(position.x, sin(time), position.z, 1);
	float start = position.x + 1.0;

	gl_Position = vec4(position.x, 0.5 * sin(time + 2 * start * PI), position.z, 1);
}
