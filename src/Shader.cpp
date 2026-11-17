#include "Shader.h"

#include <fstream>

#include "GL/glew.h"
#include "iostream"

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

inline unsigned int CompileShader(unsigned int type,
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

unsigned int CreateShader(const std::string& vertexShader,
                          const std::string fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);
  glDeleteShader(vs);
  glDeleteShader(fs);
  return program;
}
