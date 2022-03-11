#pragma once

#include <string>
#include <glm/glm.hpp>
#include <iostream>
using namespace glm;

#define LOG(expr) (std::cerr << #expr " = " << (expr) << "\n")
#define ASSERT(expr, ...) _assert((bool)(expr), #expr __VA_ARGS__)

void _assert(bool val, const char *expr, const char *extra = nullptr);
void die(const char *err);
std::string read_file(const char *fn);
float rand_float(float l, float r);
float mmod(float a, float b);
vec3 generate_color(float states);
