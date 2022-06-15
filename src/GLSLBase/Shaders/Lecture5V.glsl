#version 450

in vec3 position;

uniform float time;

out vec2 textCoord;

const float PI = 3.141592;

void main()
{
//	gl_Position = vec4(position, 1);
//	textCoord = vec2((position.x + 1.0) / 2.0, (position.y + 1.0) / 2.0);
	float start = position.x + 1.0;
	gl_Position = vec4(position.x, 0.5 * sin(time + 2 * PI * start), position.z, 1);
}
