#version 450

layout(location=0) out vec4 FragColor;
in vec4 color;

const float pi = 3.14;

uniform vec3 points[10];
uniform float time;

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

vec4 DrawCircles()
{
	vec4 retColor = vec4(0);

	for (int i = 0; i < 10; ++i)
	{
		float d = distance(points[i].xy, color.xy);

		if (d < time)
		{
			retColor += vec4(sin(10 * d * 4 * pi - time * 100));
		}
	}

	return retColor;
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

vec4 DrawMultipleCircles(float dist)
{
	float temp = sin(10 * dist * 2 * pi);

	return vec4(temp);
}

vec4 RadarCircle()
{
	float d = distance(vec2(0.5, 0), color.xy);
	float sinVal = clamp(pow(sin(d * 2 * pi - time * 100), 4), 0, 1);
	vec4 ret = vec4(0.5 * sinVal);

	for (int i = 0; i < 10; ++i)
	{
		float dis = distance(points[i].xy, color.xy);

		if (dis < 0.1)
		{
			ret += vec4(0, 20 * sinVal * 0.1 - dis, 0, 0);
		}
	}

	return ret;
}

void main()
{
	float dist = distance(color.xy, vec2(0.5, 0.5)); // 0 ~ 0.5
	
	//FragColor = DrawCircle(dist);
	//FragColor = DrawCircleLine(dist);
	//FragColor = CrossPattern();
	//FragColor = DrawMultipleCircles(dist);
	//FragColor = DrawCircles();
	FragColor = RadarCircle();
}
