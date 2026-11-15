#pragma once

#define GLCall(x) \
  GLClearError(); \
  x;              \
  Assert(GLCheckError(#x, __FILE__, __LINE__))

void FillOpenGlErrorMap();

void Assert(bool exp);

void GLClearError();

bool GLCheckError(const char* function, const char* file, int line);
