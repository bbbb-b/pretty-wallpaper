#version 430 core
out vec4 vert_color;
out vec2 vert_pos;

layout(location = 0) in vec2 in_pos;
layout(location = 1) in vec3[3] in_cols;
layout(location = 2) in float color_change_time;

layout(location = 0) uniform float local_time;


void main()
{
	//vec2 new_uv = mirror_mod((frag_uv + u_time) * 10, 1);
	//new_uv = nsigmoid(new_uv);
	//vec3 new_col = vec3(1, new_uv) * 0.5;
	//vec3 new_col = vec3(new_uv, 1);
	//color = vec4(texture(tex, frag_uv).rgb, 1);
	//color = vec4(frag_uv * u_time, 0, 1);
	//color = vec4(frag_uv, 0, 1);
	//float dist = 1 - distance(frag_uv, vec2(0, 0));
	//color = vec4(dist, dist, dist, 1) * texture(tex, frag_uv+1);
	//color = vec4(1 - mix(texture(tex, frag_uv), texture(tex2, frag_uv), mod(u_time, 1)).rgb, 1);
	//float time_val 
	//int col_idx = 
	float calculated_alpha = abs(mod(local_time+5, color_change_time * 2) / color_change_time - 1);
	vert_color = vec4(in_cols[0], calculated_alpha);
	vert_pos = in_pos;
	//vert_color = vec4(in_col, 0.01);
	gl_Position = vec4(in_pos, 0, 1);
} 


