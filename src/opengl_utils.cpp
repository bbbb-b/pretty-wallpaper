#include <glad/glad.h>
#include <iostream>
#include "utils.hpp"
#include "stb_image.h"

void check_shader_status(unsigned int shader) {
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char data[0x200];
		glGetShaderInfoLog(shader, 0x200, nullptr, data);
		die(data);
	}
}

void check_program_status(unsigned int program) {
	int success;
	glGetProgramiv(program, GL_COMPILE_STATUS, &success);
	if (!success) {
		char data[0x200];
		glGetProgramInfoLog(program, 0x200, nullptr, data);
		die(data);
	}
}

unsigned int create_shader(unsigned int shader_type, const char *src_file) {
	GLuint shader = glCreateShader(shader_type);
	std::string shader_src = read_file(src_file);
	const char* shader_src_str = shader_src.c_str();
	glShaderSource(shader, 1, &shader_src_str, nullptr);
	glCompileShader(shader);
	check_shader_status(shader);
	return shader;
}

unsigned int load_texture(const char *path) {
	unsigned int texture;
	int twidth;
	int theight;
	int tchannel_cnt;
	unsigned char *tdata = stbi_load(path, &twidth, &theight, &tchannel_cnt, 3); 
	ASSERT(tdata);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(tdata);
	return texture;
}

unsigned int create_compute_program(const char* fn) {
	unsigned int compute_shader = create_shader(GL_COMPUTE_SHADER, fn);
	unsigned int prog = glCreateProgram();
	glAttachShader(prog, compute_shader);
	glLinkProgram(prog);
	check_program_status(prog);
	glDeleteShader(compute_shader);
	return prog;
}

unsigned int create_shader_program(const char* fn1, const char *fn2) {
	unsigned int vertex_shader = create_shader(GL_VERTEX_SHADER, fn1);
	unsigned int fragment_shader = create_shader(GL_FRAGMENT_SHADER, fn2);
	unsigned int shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	check_program_status(shader_program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	return shader_program;
}
