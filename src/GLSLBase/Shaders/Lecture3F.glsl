#version 450

layout(location=0) out vec4 FragColor;

in vec4 outColor;
uniform vec4 uColor;

void main()
{
	FragColor = vec4(outColor);
}
