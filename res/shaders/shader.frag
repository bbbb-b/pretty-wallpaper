#version 430 core

in vec4 vert_color;
in vec2 vert_pos;
out vec4 out_color;

void main()
{
	float alpha = vert_color.a;// * (1 - (pow((distance(vec2(0, 0), abs(vert_pos)) / sqrt(2)), 1.4)));
	out_color = vec4(vert_color.xyz * alpha, 1);
}
