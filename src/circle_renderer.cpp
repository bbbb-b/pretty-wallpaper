#include "circle_renderer.hpp"

#include "utils.hpp"
#include "opengl_utils.hpp"

#include <time.h>
#include <iostream>


Circle CircleSkeleton::create_circle(double t, uvec2 render_size) const {
	Circle circle;
	circle.pos.x = mmod(start_pos.x - size + t * speed,
		render_size.x - size * 2) + circle.size;
	circle.pos.y = mmod(start_pos.y - size + t * speed,
		render_size.y - size * 2) + circle.size;
	circle.size = size;
	vec3 curr_color = colors[(int)floor(t / color_change_time) % colors.size()];
	vec3 next_color = colors[((int)floor(t / color_change_time) + 1) % colors.size()];
	float color_diff = fmod(t, color_change_time) / color_change_time;
	//LOG(curr_color);
	circle.color = curr_color + (next_color - curr_color) * (color_diff);
	return circle;
}

CircleSkeleton CircleSkeleton::generate(double start_time, uvec2 render_size,
		int color_cycle_count) {
	CircleSkeleton circle;
	circle.size = rand_float(30, 60) * 0.7;
	circle.speed = rand_float(50, 400) * 0.7;
	circle.start_pos.x = rand_float(circle.size, render_size.x - circle.size);
	circle.start_pos.y = rand_float(circle.size, render_size.y - circle.size);
	circle.colors.resize(color_cycle_count);
	for (vec3 & color : circle.colors) {
		color = generate_color(1);
		//color.x = ge
		//color.y = round(rand_float(0, 1));
		//color.z = round(rand_float(0, 1));
	}
	circle.start_time = start_time;
	circle.color_change_time = rand_float(0.5, 2);
	//circle.color_change_time = 5;
	return circle;
}

bool CircleRenderer::init_rendering() {
	for (int i = 0; i < circle_count; i++) {
		circle_skeletons.push_back(CircleSkeleton::generate(0, RENDER_SIZE,
			color_cycle_count));
	}

	{
		p_draw_texture = create_shader_program(
			"res/shaders/draw_texture.vert", "res/shaders/draw_texture.frag");
		p_clear_circles = create_compute_program("res/shaders/clear_circles.comp");
		p_add_circle = create_compute_program("res/shaders/add_circle.comp");
		p_display_circles = create_compute_program("res/shaders/display_circles.comp");
	}
	{
		glGenBuffers(1, &ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER,
			RENDER_SIZE.x * RENDER_SIZE.y * sizeof(float) * 8, nullptr, GL_DYNAMIC_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
	}
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		float vertices[2*6] = {
			-1.0f, -1.0f,
			-1.0f,  1.0f,
			 1.0f,  1.0f,
			 1.0f,  1.0f,
			 1.0f, -1.0f,
			-1.0f, -1.0f,
		};
		assert (sizeof(vertices) == 48);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);
	}

	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
			RENDER_SIZE.x, RENDER_SIZE.y, 0, GL_RGBA, GL_FLOAT, nullptr);
		glBindImageTexture(0, texture, 0, GL_FALSE,
			0, GL_WRITE_ONLY, GL_RGBA32F); // change 0 to 2?

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	//int te
	
	/*
	glBindBuffer(GL_ARRAY_BUFFER, vbo_time);
	glBufferData(GL_ARRAY_BUFFER, sizeof(times), times, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(1);
*/

	return true;
}

void CircleRenderer::draw_frame() {
	try_update_rand();
	glClearColor(0.0, 0.0, 0, 0.2);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//return;
	glBindVertexArray(vao);
	glUseProgram(p_draw_texture);
	//glUniform3f(1, 0.5, 0, 1);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glFlush();
	//return;
	clear_circles();
	for (const auto &circle_skeleton : circle_skeletons) {
		add_circle(circle_skeleton.create_circle(local_time, RENDER_SIZE));
	}
	draw_circles();
}

void CircleRenderer::clear_circles() {
	glUseProgram(p_clear_circles);
	glDispatchCompute(RENDER_SIZE.x / 16, RENDER_SIZE.y / 16, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void CircleRenderer::add_circle(const Circle &circle) {
	glUseProgram(p_add_circle);
	int circle_x = glGetUniformLocation(p_add_circle, "circle_x");
	glUniform1f(circle_x, circle.pos.x);
	int circle_y = glGetUniformLocation(p_add_circle, "circle_y");
	glUniform1f(circle_y, circle.pos.y);
	int circle_size = glGetUniformLocation(p_add_circle, "circle_size");
	glUniform1f(circle_size, circle.size);
	int circle_color = glGetUniformLocation(p_add_circle, "circle_color");
	glUniform3f(circle_color, circle.color.x, circle.color.y, circle.color.z);
	glDispatchCompute(RENDER_SIZE.x / 16, RENDER_SIZE.y / 16, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	
}

void CircleRenderer::draw_circles() {
	glUseProgram(p_display_circles);
	glDispatchCompute(RENDER_SIZE.x / 16, RENDER_SIZE.y / 16, 1);
	//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glUseProgram(p_draw_texture);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void CircleRenderer::try_update_rand() {
	glUseProgram(p_add_circle);
	double curr_rand_time = local_time - std::fmod(local_time, freq_rand_duration);
	{
		int location = glGetUniformLocation(p_add_circle, "interpolate_rand"); 
		glUniform1f(location, (local_time - curr_rand_time) / freq_rand_duration);
	}
	if (curr_rand_time == last_rand_update)
		return;
	float last_rand;
	int location1 = glGetUniformLocation(p_add_circle, "frequent_rand1"); // curr
	int location2 = glGetUniformLocation(p_add_circle, "frequent_rand2"); // next
	float curr_rand;
	if (last_rand_update <= 0)
		curr_rand = rand_float(0, 1);
	else
		glGetUniformfv(p_add_circle, location2, &curr_rand);
	float next_rand = rand_float(0, 1); // doesnt matter too much
	glUniform1f(location1, curr_rand);
	glUniform1f(location2, next_rand);
	last_rand_update = curr_rand_time;

}
