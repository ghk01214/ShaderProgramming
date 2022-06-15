#version 450

in vec3 position;

out vec2 textCoord;

void main()
{
	gl_Position = vec4(position, 1);
	textCoord = vec2((position.x + 1.0) / 2.0, (position.y + 1.0) / 2.0);
}
