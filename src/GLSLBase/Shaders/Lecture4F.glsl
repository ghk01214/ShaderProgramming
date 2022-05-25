#version 450

layout(location=0) out vec4 FragColor;
in vec4 color;

const float pi = 3.14;

vec4 CrossPattern()
{
	vec4 ret = vec4(1, 1, 1, 1);

	float x = sin((color.x * 2 * pi + pi / 4) * 10);
	float y = sin((color.y * 2 * pi + pi / 4) * 10);
	float result = max(x, y);

	ret = vec4(result);

	return ret;
}

vec4 DrawCircle(float dist)
{
	vec4 ret = vec4(0, 0, 0, 0);

	if (dist > 0.5)
	{
		ret = vec4(1, 1, 1, 1);
	}
	
	return ret;
}

vec4 DrawCircleLine(float dist)
{
	vec4 ret = vec4(0, 0, 0, 0);

	if (dist > 0.48 && dist < 0.5)
	{
		ret = vec4(1);
	}
	else
	{
		ret = vec4(0);
	}

	return ret;
}
