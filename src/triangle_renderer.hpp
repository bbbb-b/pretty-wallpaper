#pragma once

#include "renderer.hpp"
#include <glm/glm.hpp>

#include <vector>

using namespace glm;

struct Vertex {
	vec2 pos;
	vec3 color[3];
	float color_change_time;
};

class TriangleRenderer : public Renderer {
	public:
	uvec2 triangle_count = uvec2(8, 3);
	protected:
	virtual bool init_rendering();
	virtual void draw_frame();

	private:
	unsigned int shader_program;
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;

};

