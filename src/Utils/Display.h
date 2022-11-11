#pragma once
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "glx.hpp"
#include "constants.hpp"

static void error_callback(int error, const char* description) {
  std::cerr << "Error[" << error << "]: " << description << "\n";
}

static void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  if (key == GLFW_KEY_S && action == GLFW_PRESS) {
    std::puts("Export current display");
    char export_filename[] = "export.png";
    saveImage(export_filename, window);
  }
}
namespace lines {
mat3x3 vertex_transform_2d(float width, float height) {
  return mat3x3{{{{2.f / width, 0.f, 0.f}}, {{0.f, -2.f / height, 0.f}}, {{-1.f, 1.f, 1.f}}}};
}
static const char* const vertex_shader_text = R"#(
#version 330 core
uniform mat3 transform;
in vec2 vPos;
in vec3 vCol; // Specify a vertex attribute for color
out vec3 color;
void main()
{
    gl_Position = vec4(transform * vec3(vPos, 1.0), 1.0);
	color = vCol; // pass the color along to the fragment shader
}
)#";

static const char* const fragment_shader_text = R"#(
#version 330 core

in vec3 color;
out vec4 fragColor;
void main() {
    // Set the fragment color to the color passed from the vertex shader
    fragColor = vec4(color, 1.0);
}
)#";
}  // namespace lines