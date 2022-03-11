#include "triangle_renderer.hpp"
#include "opengl_utils.hpp"
#include "utils.hpp"

std::vector<float> gen_positions(int cnt) {
	//ASSERT(cnt >= 2);
	std::vector<float>ret;
	//ret.push_back(rand_float(-1, -1-(float)1/(float)(cnt*2)));
	//ret.push_back(rand_float(1, 1+(float)1/(float)(cnt*2)));
	for (int i = 0; i < cnt; i++) {
		if (i <= 1)
			ret.push_back(i*2-1);
		else
			ret.push_back(rand_float(-1, 1));
	}
	std::sort(ret.begin(), ret.end());
	return ret;
}

bool TriangleRenderer::init_rendering() {
	shader_program = create_shader_program(
		"res/shaders/shader.vert", "res/shaders/shader.frag");
	glUseProgram(shader_program);
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		std::vector<Vertex> vertices;
		std::vector<Vertex> real_vertices;
		std::vector<unsigned int> indices;
		//std::vector<vec3> color_vertices;
		std::vector<unsigned int> color_indices;
		int y_cnt = triangle_count.y;
		int x_cnt = triangle_count.x;
		std::vector<float> y_positions {-1, 1};//= gen_positions(y_cnt-1);
		for (int y = 1; y < y_cnt; y++) {
			y_positions.push_back(((float)y / y_cnt) * 2 - 1);
		}
		//y_positions.insert(y_positions.begin(), rand_float(-1, -1-(float)1/(float)(y_cnt*2)));
		//y_positions.push_back(rand_float(1, 1+(float)1/(float)(y_cnt*2)));
		for (int y = 0; y < y_cnt; y++) {
			std::vector<float> x_positions = gen_positions(x_cnt);
			for (int x = 0; x < x_cnt; x++) {
				float y_pos;
				if (y == 0)
					y_pos = -1;
				else if (y + 1 == y_cnt)
					y_pos = 1;
				else
					y_pos = rand_float(y_positions[y-1], x_positions[x]);
				//float y_pos = (y == 0 || y + 1 == y_cnt) ? gen_positions
				vec2 pos = vec2(y_pos, x_positions[x]);
				std::swap(pos.x, pos.y);
				//pos.y = y_range.y;
				//vec2 pos = vec2(((float)x / (float)x_cnt) * 2 - 1, y_range.x);
				//vec3 col = generate_color(100);
				//col.z = col.y = 0;
				//col = vec3(1, 0, 0);
				vertices.push_back({pos});
			}
			std::sort(vertices.begin() + x_cnt * y, vertices.end(),
				[](const Vertex &lhs, const Vertex &rhs) { return lhs.pos.x < rhs.pos.x;});
		}
		for (int y = 1; y < y_cnt; y++) {
			for (int x = 0; x + 1 < x_cnt; x++) {
				indices.push_back((y-1) * x_cnt + x + 0);
				indices.push_back((y-1) * x_cnt + x + 1);
				indices.push_back((y-0) * x_cnt + x + 0);

				indices.push_back((y-1) * x_cnt + x + 1);
				indices.push_back((y-0) * x_cnt + x + 0);
				indices.push_back((y-0) * x_cnt + x + 1);
				vec3 last_col[3];
				float last_col_change_time = 0;
				for (int i = 6; i >= 1; i--) {
					Vertex v = vertices[indices[indices.size()-i]];
					if (i % 3 == 0) {
						for (int j = 0; j < 3; j++)
							last_col[j] = generate_color(10) * rand_float(0.3, 0.6);
						//last_col = vec3(1, 0, 0) * rand_float(0.2, 0.6);
						last_col_change_time = rand_float(1, 5);
					}
					for (int j =0; j < 3; j++)
						v.color[j] = last_col[j];
					v.color_change_time = last_col_change_time;
					real_vertices.push_back(v);
				}
				//color_indices.push_back(generate_color(2));
			}
		}
		/*
		Vertex vertices[4] = {
			Vertex({vec2(-1.0f, -1.0f), vec3(0, 0, 0)}),
			Vertex({vec2(-1.0f,  1.0f), vec3(0, 0, 1)}),
			Vertex({vec2( 1.0f,  -1.0f), vec3(0, 1, 0)}),
			Vertex({vec2( 1.0f,  1.0f), vec3(0, 1, 1)}),

			//Vertex({vec2( 1.0f, -1.0f), vec3(1, 0, 0)}),
			//Vertex({vec2(-1.0f, -1.0f), vec3(1, 0, 1)})
		};
		int indices[6] = {
			0, 1, 2,
			3, 1, 2,
		};*/
		//assert (sizeof(vertices) == 48);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			//sizeof(unsigned int) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(Vertex) * real_vertices.size(), real_vertices.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				(void*)offsetof(Vertex, color_change_time));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}
	return true;
}

void TriangleRenderer::draw_frame() {
	//try_update_rand();
	glClearColor(0.1, 0.1, 0.1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//return;
	glBindVertexArray(vao);
	glUseProgram(shader_program);
	glUniform1f(0, local_time);
	//glUniform3f(1, 0.5, 0, 1);
	//glDrawElements(GL_TRIANGLES, 6 * (triangle_count.y-1) * (triangle_count.x) * 2, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6 * (triangle_count.y-1) * (triangle_count.x) * 2);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//glFlush();
	//return;
}
