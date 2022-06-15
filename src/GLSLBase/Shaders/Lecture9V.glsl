#version 450

in vec3 position;
out vec4 color;

uniform float time;

const float PI = 3.141592;

void Flag()
{
	float xdis = position.x + 0.5;
	float dis = distance(position.xy, vec2(-0.5, 0.0));
	gl_Position =  vec4((position.x + xdis * sin(dis * 2.0 * PI - time)) * 0.5, (position.y + xdis * sin(dis * 2.0 * PI - time)) * 0.5,  position.z, 1);
	color = vec4((sin((position.x + 0.5) * 2.0 * PI - time) + 1.0) / 2.0);
}

void main()
{
	Flag();
}
