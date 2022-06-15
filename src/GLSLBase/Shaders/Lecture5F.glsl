#version 450

layout(location = 0) out vec4 FragColor;
in vec2 textCoord;

uniform float time;

const float PI = 3.141592;

void main()
{
	float sinVal = 0.5 * sin(2.0 * textCoord.x * 2 * PI + time);

	if (textCoord.y * 2.0 - 1.0 < sinVal &&
		textCoord.y * 2.0  - 1.0 > sinVal - 0.01)
	{
		FragColor = vec4(1);
	}
	else
	{
		FragColor = vec4(0);
	}
}