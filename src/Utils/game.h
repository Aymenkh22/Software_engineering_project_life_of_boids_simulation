#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include "../laws/laws.hpp"
#include "Display.h"

void launch() {
  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(width, height, "Boiders", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window, key_callback);

  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);

  // FIXME: OpenGL error checks have been omitted for brevity
  // new
  ShaderProgram lines_shaderProgram = ShaderProgram_new(lines::vertex_shader_text, lines::fragment_shader_text);
  VertexArray lines_vertexArray = VertexArray_new();
  Buffer lines_buffer = Buffer_new();
  // init
  VertexArray_bind(lines_vertexArray);
  Buffer_bind(lines_buffer, GL_ARRAY_BUFFER);
  ShaderProgram_activate(lines_shaderProgram);

  const GLint transform_location2 = ShaderProgram_getUniformLocation(lines_shaderProgram, "transform");
  const GLint vpos_location2 = ShaderProgram_getAttribLocation(lines_shaderProgram, "vPos");
  const GLint vcol_location2 = ShaderProgram_getAttribLocation(lines_shaderProgram, "vCol");

  glVertexAttribPointer(vpos_location2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
  glVertexAttribPointer(vcol_location2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));

  glEnableVertexAttribArray(vpos_location2);
  glEnableVertexAttribArray(vcol_location2);

  int width{}, height{};
  glfwGetFramebufferSize(window, &width, &height);

  //************* Random initialization of agents ***************

  std::vector<Vertex> vertex_data(SIZE * 3);
  group boids = random_initializer(vertex_data);

  //************* Random initialization of agents ***************

  double previousTime = glfwGetTime();
  int frameCount = 0;

  while (!glfwWindowShouldClose(window)) {
    int width{}, height{};
    glfwGetFramebufferSize(window, &width, &height);

    auto h = static_cast<float>(height);
    auto w = static_cast<float>(width);
    const float ratio = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    double currentTime = glfwGetTime();
    frameCount++;
    // If a second has passed.
    if (currentTime - previousTime >= 1.0) {
      // We display the frame count here
      std::cout << "Current FPS : " << frameCount << '\n';
      frameCount = 0;
      previousTime = currentTime;
    }

    {
      mat3x3 transform = lines::vertex_transform_2d(width, height);

      VertexArray_bind(lines_vertexArray);
      Buffer_bind(lines_buffer, GL_ARRAY_BUFFER);
      ShaderProgram_activate(lines_shaderProgram);

      glUniformMatrix3fv(transform_location2, 1, GL_FALSE, (const GLfloat*)&transform);
      glBindVertexArray(lines_vertexArray.vertex_array);

      //************* moving the agents ***************

      magic(vertex_data, boids, w, h);

      //************* moving the agents ***************

      int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
      if (state == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        revive(boids, xpos * 2, ypos * 2);
      }

      glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(Vertex), vertex_data.data(), GL_STREAM_DRAW);
      glDrawArrays(GL_TRIANGLES, 0, vertex_data.size());
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
