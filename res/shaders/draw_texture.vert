#version 430 core
layout (location = 0) in vec2 pos;
out vec2 frag_uv;

void main()
{
	gl_Position = vec4(pos.xy , 0, 1.0);
	frag_uv = (1 + gl_Position.xy) / 2;
	//frag_uv = gl_Position.xy;
	//frag_uv.x /= 9.0 / 16.0;
}

