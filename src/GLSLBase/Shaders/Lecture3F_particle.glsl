﻿#version 450

layout(location=0) out vec4 FragColor;

in vec4 out_color;

void main()
{
	
	FragColor = vec4(out_color);
}
