#pragma once

#include <string>

struct ShaderPaserResult {
  std::string VertexSource, FragmentSource;
};

enum ParserShaderState { None = -1, Vertex = 0, Fragment = 1 };

ShaderPaserResult ParserShader(const std::string& shaderPath);

inline unsigned int CompileShader(unsigned int type,
                                  const std::string& sourceCode);

unsigned int CreateShader(const std::string& vertexShader,
                          const std::string fragmentShader);
