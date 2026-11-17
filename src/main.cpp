#include <cassert>
#include <iostream>
#include <ostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "IndexBuffer.h"
#include "Render.h"
#include "Shader.h"
#include "VertexBuffer.h"

int main() {
  GLFWwindow* window;
  FillOpenGlErrorMap();

  if (!glfwInit()) {
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHintString(GLFW_WAYLAND_APP_ID, "opengl-window");

  window = glfwCreateWindow(455, 457, "Hello OpenGl", NULL, NULL);
  // glfwWindowHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if (const auto err = glewInit();
      err != GLEW_OK && err != GLEW_ERROR_NO_GLX_DISPLAY) {
    std::cout << "ERR:" << glewGetErrorString(err) << std::endl;
  }

  std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

  // clang-format off
  float positions[] = {
      -0.5, -0.5, // 0
      0.5, -0.5, // 1
      0.5, 0.5, // 2
      -0.5, 0.5, // 3
      0.0,1.0 // 4
  };
  // clang-format on

  unsigned int indices[] = {0, 1, 2, 2, 4, 3, 2, 3, 0};
  unsigned int vao;

  GLCall(glGenVertexArrays(1, &vao));
  GLCall(glBindVertexArray(vao));

  VertexBuffer vb(positions, sizeof(positions));

  GLCall(glEnableVertexAttribArray(0));
  GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

  IndexBuffer ib(indices, sizeof(indices));

  ShaderPaserResult source = ParserShader("res/shaders/Base.shader");
  unsigned int program =
      CreateShader(source.VertexSource, source.FragmentSource);

  GLCall(glUseProgram(program));

  int uColor = glGetUniformLocation(program, "u_Color");
  GLCall(glUniform4f(uColor, 0, 0.5, 0.5, 1));

  GLCall(glBindVertexArray(0));
  GLCall(glUseProgram(0));

  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

  float inc = 0.05;
  float g = 0;

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    if (g > 1) {
      inc = -0.05;
    } else if (g < 0) {
      inc = 0.05;
    }
    g += inc;

    GLCall(glUseProgram(program));
    GLCall(glUniform4f(uColor, 0, g, 0.5, 1));
    GLCall(glBindVertexArray(vao));
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int),
                          GL_UNSIGNED_INT, NULL));
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteProgram(program);

  glfwTerminate();
  return 0;
}
