#include "circle_renderer.hpp"
#include "triangle_renderer.hpp"

int main() {
	//TriangleRenderer r;
	CircleRenderer r;
	//r.fps = 30000;
	//r.fps = 144;
	//r.is_window_wallpaper = true;
	bool s = r.start();
	return s ? 0 : 1;
	//std::cerr << s << "\n";
}
