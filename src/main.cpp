#include <cassert>
#include <fstream>
#include <iostream>
#include <ostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Render.h"

struct ShaderPaserResult {
  std::string VertexSource, FragmentSource;
};

enum ParserShaderState { None = -1, Vertex = 0, Fragment = 1 };

ShaderPaserResult ParserShader(const std::string& shaderPath) {
  ParserShaderState state = ParserShaderState::None;
  std::ifstream file(shaderPath);
  std::string line;
  std::string result[2];
  if (file.is_open()) {
    while (std::getline(file, line)) {
      if (line.find("shader") != std::string::npos) {
        if (line.find("vertex") != std::string::npos) {
          state = ParserShaderState::Vertex;
        }
        if (line.find("fragment") != std::string::npos) {
          state = ParserShaderState::Fragment;
        }
      } else {
        switch (state) {
          case None: {
            break;
          }
          case Vertex: {
            result[0] += line + "\n";
            break;
          }
          case Fragment: {
            result[1] += line + "\n";
            break;
          }
        }
      }
    }
  }
  return ShaderPaserResult{result[0], result[1]};
}

static unsigned int CompileShader(unsigned int type,
                                  const std::string& sourceCode) {
  unsigned int id = glCreateShader(type);
  const char* str = sourceCode.c_str();
  glShaderSource(id, 1, &str, NULL);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    // ShaderError
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compiled "
              << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
              << " shader " << message << std::endl;
    glDeleteShader(id);
    return 0;
  }
  return id;
}

static unsigned int CreateShader(const std::string& vertexShader,
                                 const std::string fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);
  glDeleteShader(vs);
  glDeleteShader(vs);
  return program;
}

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

  std::cout << glGetString(GL_VERSION) << std::endl;

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

  unsigned int buffer;
  GLCall(glGenBuffers(1, &buffer));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
  GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions,
                      GL_STATIC_DRAW));

  unsigned int ibo;
  GLCall(glGenBuffers(1, &ibo));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                      GL_STATIC_DRAW));

  GLCall(glEnableVertexAttribArray(0));
  GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

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

    // GLCall(glEnableVertexAttribArray(0));

    GLCall(glBindVertexArray(vao));
    // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    // GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    // GLCall(
    //     glVertexAttribPointwr(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2,
    //     0));

    GLCall(glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int),
                          GL_UNSIGNED_INT, NULL));
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteProgram(program);

  glfwTerminate();
  return 0;
}
