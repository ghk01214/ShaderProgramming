#version 450

// CPU에서 GPU로 값을 넘길 때는 vec4 형식으로 넘긴다.(더 효율적)
// float = vec1과 같으므로 float형 여러개를 vec로 묶어서 보내는 것이 최적화 면에서 더 좋다.
in vec3 a_Position;
in vec3 velocity;		// 속도
in float emit_time;		// 생성 시간
in float life_time;		// 생명 주기
in float amp;			// 진폭
in float period;		// 주기
in float value;			// 값
in vec4 in_color;
out vec4 out_color;
out vec2 text_coord;

uniform float time;		// 진행 시간
uniform vec3 accel;		// 가속도

bool loop = true;

const float pi = 3.14;
const vec3 gravity = vec3(0, -1.2, 0);		// 중력가속도
mat3 rotate_matrix =  mat3(0, -1, 0, 1, 0, 0, 0, 0, 0);

void main()
{
	vec3 new_pos;
	float t = time - emit_time;
	float tt = t * t;

	if (t > 0)
	{
		vec3 new_accel = gravity + accel;
		new_pos.x = sin(value * 2 * pi);
		new_pos.y = cos(value * 2 * pi);
		new_pos.z = 0;

		new_pos = a_Position + new_pos;

		float fractional = fract(t / life_time);		// fract() 소수점 아래 부분만 구해주는 셰이더 자체 함수
		t = fractional * life_time;
		tt = t * t;

		new_pos = new_pos + new_accel * t + 0.5 * accel * tt;

		vec3 rotate_vec = normalize(new_accel * rotate_matrix);
		new_pos = new_pos + amp * t * rotate_vec * sin(period * t * 2.0 * pi);
		
		out_color = in_color * (1.0 - fractional);
	}
	else
	{
		new_pos = vec3(-1000000, -1000000, -100000);
		out_color = vec4(0, 0, 0, 0);
	}
	
	gl_Position = vec4(new_pos, 1);
	text_coord = a_Position.xy;
}
