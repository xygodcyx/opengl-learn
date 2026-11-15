#include "Render.h"

#include <cassert>
#include <iostream>
#include <map>
#include <string>

#include "GL/glew.h"

#define GLCall(x) \
  GLClearError(); \
  x;              \
  Assert(GLCheckError(#x, __FILE__, __LINE__))

std::map<unsigned int, std::string> OpenGLFunctionErrorMap;

void FillOpenGlErrorMap() {
  OpenGLFunctionErrorMap[GL_INVALID_ENUM] =
      "'GL_INVALID_ENUM' An unacceptable value is specified for an enumerated argument. The "
      "offending command is ignored and has no other side effect than to set the error flag.";

  OpenGLFunctionErrorMap[GL_INVALID_VALUE] =
      "'GL_INVALID_VALUE' A numeric argument is out of range. The offending command is ignored and "
      "has no other side effect than to set the error flag.";

  OpenGLFunctionErrorMap[GL_INVALID_OPERATION] =
      "'GL_INVALID_OPERATION' The specified operation is not allowed in the current state. The "
      "offending command is ignored and has no other side effect than to set the error flag.";

  OpenGLFunctionErrorMap[GL_INVALID_FRAMEBUFFER_OPERATION] =
      "GL_INVALID_FRAMEBUFFER_OPERATION The framebuffer object is not complete. The offending "
      "command is ignored and has no other side effect than to set the error flag.";

  OpenGLFunctionErrorMap[GL_OUT_OF_MEMORY] =
      "'GL_OUT_OF_MEMORY' There is not enough memory left to execute the command. The state of "
      "the "
      "GL is undefined, except for the state of the error flags, after this error is recorded.";

  OpenGLFunctionErrorMap[GL_STACK_UNDERFLOW] =
      "'GL_STACK_UNDERFLOW' An attempt has been made to perform an operation that would cause an "
      "internal stack to underflow.";

  OpenGLFunctionErrorMap[GL_STACK_OVERFLOW] =
      "'GL_STACK_OVERFLOW' An attempt has been made to perform an operation that would cause an "
      "internal stack to overflow.";
}

void Assert(bool exp) {
  assert(exp);
}

void GLClearError() {
  while (glGetError() != GL_NO_ERROR) {
  }
}

bool GLCheckError(const char* function, const char* file, int line) {
  while (GLenum error = glGetError()) {
    std::cout << "[OpenGl Error] (" << error << ": " << OpenGLFunctionErrorMap[error] << ")"
              << "\ncall: " << function << "\nin: " << file << ":" << line << std::endl;
    return false;
  }
  return true;
}
