#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include "utils.hpp"


void _assert(bool val, const char *expr, const char *extra) {
	if (!val) {
		std::cerr << "Assert '" << expr << "' failed";
		if (extra)
			std::cerr << ": ";
		die(extra);
	}
}

float mmod(float a, float b) {
	return fabs(fmod(a, b*2) - b);
}

void die(const char *err) {
	if (err)
		std::cerr << err << "\n";
	exit(1);
}

std::string read_file(const char *fn) {
	std::ifstream file(fn);
	ASSERT(file);
	std::stringstream ss;
	ss << file.rdbuf();
	file.close();
	return ss.str();
}


float rand_float(float l, float r) { // in range [l:r)
	float base = (float)rand() / (float)RAND_MAX;
	return l + base * ((r) - l);
}


vec3 generate_color(float states) {
	vec3 col;
	col.x = std::round(rand_float(0, states)) / states;
	col.y = std::round(rand_float(0, states)) / states;
	col.z = std::round(rand_float(0, states)) / states;
	return col;
}
