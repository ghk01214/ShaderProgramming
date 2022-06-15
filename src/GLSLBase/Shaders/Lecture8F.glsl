#version 450

layout(location = 0) out vec4 FragColor;
in vec2 outTexPos;

uniform sampler2D texSampler[2];
uniform float time;

vec4 Reverse(int index)
{
	vec2 newTexPos = vec2(outTexPos.x, 1.0 - outTexPos.y);

	return texture(texSampler[index], newTexPos);
}

vec4 Mirror(int index)
{
	vec2 newTexPos = vec2(outTexPos.x, abs((outTexPos.y - 0.5) * 2));

	return texture(texSampler[index], newTexPos);
}

vec4 Rotate(int index)
{
	// fract = 소수 부분만 남기는 함수
	// floor = 정수 부분만 남기는 함수
	vec2 newTexPos = vec2(fract(outTexPos.x * 3.0), (outTexPos.y + floor(outTexPos.x * 3.0)) /3.0);

	return texture(texSampler[index], newTexPos);
}

vec4 Block(int index)
{
	vec2 newTexPos = vec2(fract(outTexPos.x * 2.0) + floor(outTexPos.y * 2.0) * 0.5, fract(outTexPos.y * 2.0));

	return texture(texSampler[index], newTexPos);
}

vec4 RGBnCheck()
{
	vec2 newTexPos = vec2(fract(outTexPos.x * 2.0), outTexPos.y);

	if (outTexPos.x > 0.5)
	{
		return texture(texSampler[1], newTexPos);
	}
	else
	{
		return texture(texSampler[0], newTexPos);
	}
}

vec4 Mix()
{
	vec2 newTexPos = outTexPos;
	vec2 newTexPos1 = vec2(fract(outTexPos.x + time), outTexPos.y);

	return texture(texSampler[0], newTexPos) * texture(texSampler[1], newTexPos1);
}

void main()
{
	//FragColor = texture(texSampler[0], outTexPos);
	//FragColor = Reverse(0);
	//FragColor = Mirror(0);
	//FragColor = Rotate(0);
	//FragColor = Block(0);
	//FragColor = RGBnCheck();
	FragColor = Mix();
}