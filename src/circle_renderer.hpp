#pragma once

#include "renderer.hpp"
#include <glm/glm.hpp>

#include <vector>

using namespace glm;


struct Circle {
	vec2 pos;
	float size;
	vec3 color;
};

struct CircleSkeleton {
	vec2 start_pos;
	float size;
	float speed;
	std::vector<vec3> colors;
	float color_change_time;
	double start_time;

	Circle create_circle(double t, uvec2 render_size) const;
	static CircleSkeleton generate(double start_time, uvec2 render_size,
		int color_cycle_count);
};

class CircleRenderer : public Renderer {

	public:
	int color_cycle_count = 5;
	int circle_count = 7;
	double freq_rand_duration = 0.5; // changes every 0.5 seconds
	protected:
	virtual bool init_rendering();
	virtual void draw_frame();

	private:
	double last_rand_update = -1e7;
	uvec2 RENDER_SIZE = uvec2(2560 / 2, 1440 / 2);
	std::vector<CircleSkeleton> circle_skeletons;
	unsigned int vao;
	unsigned int vbo;
	unsigned int ssbo;
	unsigned int texture;
	unsigned int p_clear_circles;
	unsigned int p_add_circle;
	unsigned int p_display_circles;
	unsigned int p_draw_texture;

	void clear_circles();
	void add_circle(const Circle &circle);
	void draw_circles();
	void try_update_rand();

};
