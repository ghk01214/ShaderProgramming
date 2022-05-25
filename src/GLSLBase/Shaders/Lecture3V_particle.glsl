#version 450

in vec3 a_Position;
in vec3 velocity;		// 속도
in float emit_time;		// 생성 시간
in float life_time;		// 생명 주기
in float amp;			// 진폭
in float period;		// 주기

uniform float time;		// 진행 시간
uniform vec3 accel;		// 가속도

float pi = 3.14;
bool loop = true;

void main()
{
	vec3 new_pos;
	float t = time - emit_time;
	float tt = t * t;

	if (t > 0)
	{
		float temp = t / life_time;
		float fractional = fract(temp);		// fract() 소수점 아래 부분만 구해주는 셰이더 자체 함수
		t = fractional * life_time;
		tt = t * t;

		new_pos.x = a_Position.x + velocity.x * t + 0.5 * accel.x * tt;
		new_pos.y = a_Position.y + amp * sin(period * t * 2.0 * pi);
		new_pos.z = 0;
	}
	else
	{
		new_pos = vec3(-1000000, -1000000, -100000);
	}
	
	gl_Position = vec4(new_pos, 1);
}
