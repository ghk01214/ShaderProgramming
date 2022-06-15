#version 450

layout(location = 0) out vec4 FragColor;
in vec2 texPos;

uniform sampler2D texSampler;

void main()
{
	FragColor = texture(texSampler, texPos);
}