#version 450

layout(location = 0) out vec4 FragColor;
in vec2 textCoord;

uniform float time;

const float PI = 3.141592;

void main()
{
//	float sinVal = sin(textCoord.x * 2 * PI);
//	FragColor = vec4(textCoord, 0, 1);
	FragColor = vec4(1, 1, 1, 1);
}