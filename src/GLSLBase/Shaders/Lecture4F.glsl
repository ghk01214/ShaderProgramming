#version 450

layout(location=0) out vec4 FragColor;
in vec4 color;

void main()
{
	float dist = distance(color.xy, vec2(0.5, 0.5));
	vec4 new_color = vec4(0, 0, 0, 0);

	if (dist > 0.5)
	{
		new_color = vec4(1, 1, 1, 1);
	}
	
	FragColor = new_color;

//	if (color.y > 0.5)
//	{
//		FragColor = vec4(1);
//	}
//	else
//	{
//		FragColor = vec4(0);
//	}
}
