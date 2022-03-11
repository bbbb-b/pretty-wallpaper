#pragma once

// rendering
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

#include <glm/glm.hpp>

using namespace glm;

/* how it works
contructor inits glfw and sets defaults
after that, public variables can be changed
then, init() does other config stuff based on those variables

*/
class Renderer {

	public: // config stuff, defaults set by constructor, can be changed later
	GLFWmonitor *monitor = nullptr;
	double fps = -1; 
	const char *window_name = "WINDOW_NAME";
	bool is_window_wallpaper = true;
	Renderer() {
		if (ensure_glfw_active())
			monitor = glfwGetPrimaryMonitor();
		//std::cerr << std::fixed << std::setw(2);
	}


	protected:
	double delta_time;
	double local_time; // cant use 'time'
	ivec2 window_pos;
	ivec2 window_size;
	GLFWwindow *window = nullptr;

	private:
	virtual bool init_rendering();
	virtual void draw_frame();
	bool init_window();
	void run(); // shouldnt fail?

	public:
	bool start();
	

	private: // glfw stuff
	static bool ensure_glfw_active();
	static bool is_glfw_active;
	static void glfw_error_callback(int error, const char *desc);
	static void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height);
	void _glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height);
	void make_window_wallpaper();
};


