#version 450

layout(location=0) out vec4 FragColor;

uniform vec4 uColor;

void main()
{
	FragColor = vec4(uColor);
}
