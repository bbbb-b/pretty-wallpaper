#include "renderer.hpp"
#include "utils.hpp"

// not rendering
#include <iostream>
#include <thread>
#include <time.h>

bool Renderer::init_rendering() {
	return true;
}

void Renderer::draw_frame() {
	glClearColor(local_time / 5.0f, 0, local_time / 3.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}


bool Renderer::init_window() {
	if (!ensure_glfw_active()) {
		std::cerr << "ensure_glfw_active() failed\n";
		return false;
	}
	glfwGetMonitorWorkarea(monitor, &window_pos.x, &window_pos.y, &window_size.x, &window_size.y);
	if (fps <= 0)
		fps = glfwGetVideoMode(monitor)->refreshRate;
	window = glfwCreateWindow(window_size.x, window_size.y, window_name, nullptr, nullptr);
	if (window == nullptr) {
		std::cerr << "glfwCreateWindow() failed\n";
		return false;
	}
	if (is_window_wallpaper)
		make_window_wallpaper();
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "gladLoadGLLoader() failed\n";
		return false;
	}
	glViewport(0, 0, window_size.x, window_size.y);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);
	// x11 init skipping for now (fix_window())
	glfwShowWindow(window);
	glfwSetTime(0);
	local_time = 0;
	return true;
}


void Renderer::run() {
	double expected_frame_time = (double)1 / fps;

	/*
	events:
	(have time in frame left ( 1 / fps))
	draw_frame()
	sleep for time left
	*/
	glfwSwapInterval(0);
	double last_frame_times = 0;
	int last_frame_times_cnt = 0;
	double last_end_time = 0;
	double last_time_calculated = last_end_time;
	std::cerr << std::fixed;
	std::cerr.precision(3);
	while (!glfwWindowShouldClose(window)) {
		//double start_time = glfwGetTime();
		delta_time = last_end_time - local_time;
		local_time = last_end_time;
		draw_frame();
		glfwSwapBuffers(window);
		glfwPollEvents();
		double end_time = glfwGetTime();
		double sleep_time = (last_end_time + expected_frame_time) - end_time;
		last_frame_times += (end_time - last_end_time);
		last_end_time = std::max(end_time, last_end_time + expected_frame_time);
		last_frame_times_cnt += 1;
		if (last_end_time - last_time_calculated >= 1 || last_frame_times_cnt >= fps) {
			std::cerr << "Possible fps: " << (double)1 / (last_frame_times / (double) last_frame_times_cnt) << "\n";
			std::cerr << "Real fps: " << (double)last_frame_times_cnt / (last_end_time - last_time_calculated) << "\n\n";
			//LOG(last_frame_times_cnt);
			//LOG(last_end_time - last_time_calculated);
			//LOG(last_end_time);
			last_frame_times = last_frame_times_cnt = 0;
			last_time_calculated = last_end_time;
		}

		if (sleep_time > 0)
			std::this_thread::sleep_for(std::chrono::duration<double>(sleep_time));
	}
}

bool Renderer::start() {
	srand(time(nullptr));
	if (!init_window()) {
		std::cerr << "init_window() failed\n";
		return false;
	}
	if (!init_rendering()) {
		std::cerr << "init_rendering() failed\n";
		return false;
	}
	run();
	return true;
}

// glfw
//
bool Renderer::is_glfw_active = false;

bool Renderer::ensure_glfw_active() {
	if (is_glfw_active)
		return true;
	if (!glfwInit()) {
		std::cerr << "glfwInit() failed\n";
		return false;
	}
	glfwSetErrorCallback(Renderer::glfw_error_callback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	return is_glfw_active = true;
}

void Renderer::glfw_error_callback(int error, const char *desc) {
	std::cerr << "GLFW: " << desc << "\n";
}

void Renderer::glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	Renderer *renderer = (Renderer*)glfwGetWindowUserPointer(window);
	//ASSERT(renderer
	renderer->_glfw_framebuffer_size_callback(window, width, height);
}

void Renderer::_glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	window_size = ivec2(width, height);
	glViewport(0, 0, width, height);
}

void Renderer::make_window_wallpaper() {
	Window x11_win = glfwGetX11Window(window);
	Display *dpy = glfwGetX11Display();
	XSetWindowAttributes attr;
	attr.override_redirect = True;
	int status = XChangeWindowAttributes(dpy, x11_win, CWOverrideRedirect, &attr);
	XWindowAttributes full_attr;
	Window root = XDefaultRootWindow(dpy);
	//std::cerr << root << "\n";
	XReparentWindow(dpy, x11_win, root, 0, 0);
	//win->x11.overrideRedirect = GLFW_TRUE;
	//std::cerr << status << "\n";
	//char buf[200];
	//XGetErrorText(dpy, status, buf, 200);
	//std::cerr << buf << "\n";
	//XGetWindowAttributes(dpy, x11_win, &full_attr);
	//assert(full_attr.override_redirect);
	XMapWindow(dpy, x11_win);
	XMoveResizeWindow(dpy, x11_win, window_pos.x, window_pos.y,
		window_size.x, window_size.y);
	XLowerWindow(dpy, x11_win);
	//XGeyWindowAttributes(dpy, x11_win, &attr);
	//attr.override_redirect = True;
	//attr.flags |=
}
