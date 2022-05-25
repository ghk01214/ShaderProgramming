#version 450

layout(location=0) out vec4 FragColor;

in vec4 out_color;
in vec2 text_coord;

void main()
{
	float dis = distance(vec2(0, 0), text_coord);

	if (dis < 0.05)
	{
		FragColor = out_color;
	}
	else
	{
		FragColor = vec4(0, 0, 0, 0);
	}
}
