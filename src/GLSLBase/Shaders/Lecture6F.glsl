#version 450

layout(location = 0) out vec4 FragColor;
in vec2 outTexPos;

uniform sampler2D texSampler;

vec4 Reverse()
{
	vec2 newTexPos = vec2(outTexPos.x, 1.0 - outTexPos.y);

	return texture(texSampler, newTexPos);
}

vec4 Mirror()
{
	vec2 newTexPos = vec2(outTexPos.x, abs((outTexPos.y - 0.5) * 2));

	return texture(texSampler, newTexPos);
}

vec4 Rotate()
{
	// fract = 소수 부분만 남기는 함수
	// floor = 정수 부분만 남기는 함수
	vec2 newTexPos = vec2(fract(outTexPos.x * 3.0), (outTexPos.y + floor(outTexPos.x * 3.0)) /3.0);

	return texture(texSampler, newTexPos);
}

vec4 Block()
{
	vec2 newTexPos = vec2(fract(outTexPos.x * 2.0) + floor(outTexPos.y * 2.0) * 0.5, fract(outTexPos.y * 2.0));

	return texture(texSampler, newTexPos);
}

void main()
{
	//FragColor = texture(texSampler, outTexPos);
	//FragColor = Reverse();
	//FragColor = Mirror();
	//FragColor = Rotate();
	FragColor = Block();
}