#version 430 core
out vec4 color;

layout (binding = 0) uniform sampler2D tex;

in vec2 frag_uv;

layout(location = 1) uniform vec3 col;

void main()
{
	color = vec4(texture(tex, frag_uv).rgb, 1);
	//color = vec4(frag_uv, 0, 1);
	//color = vec4(col, 1);
	//color = vec4(1, 1, 0, 1);
	//color = vec4(frag_uv * u_time, 0, 1);
	//color = vec4(frag_uv, 0, 1);
	//float dist = 1 - distance(frag_uv, vec2(0, 0));
	//color = vec4(dist, dist, dist, 1) * texture(tex, frag_uv+1);
	//color = vec4(1 - mix(texture(tex, frag_uv), texture(tex2, frag_uv), mod(u_time, 1)).rgb, 1);
	//gl_FragCoord
} 

