cmake_minimum_required(VERSION 3.22)

project("pretty-wallpaper" VERSION 1.0)

include_directories(PRIVATE "include" "src" ".")
set(CMAKE_CXX_STANDARD 20)

add_subdirectory("src")

add_executable("${PROJECT_NAME}"
	"main.cpp" 
	"src/utils.cpp"
	"src/opengl_utils.cpp"
	"src/renderer.cpp"
	"src/circle_renderer.cpp"
	"src/triangle_renderer.cpp"
)

target_link_libraries("${PROJECT_NAME}" "glfw" "X11" "stb_image")
target_link_libraries("${PROJECT_NAME}" "glad")

ADD_CUSTOM_TARGET(copy_res ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/res"
	"${PROJECT_BINARY_DIR}/res"
	COMMENT "Copying resources"
)

add_dependencies("${PROJECT_NAME}" copy_res)


add_subdirectory("ext/glm")

add_library("stb_image" "stb_image.h")
set_target_properties("stb_image" PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions("stb_image" PUBLIC STB_IMAGE_IMPLEMENTATION)

