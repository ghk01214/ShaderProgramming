#version 450

in vec3 position;
out vec4 color;

void main()
{
	gl_Position = vec4(position, 1);

	vec3 new_value = position + vec3(0.5, 0.5, 0);

	color = vec4(new_value, 1);
}
