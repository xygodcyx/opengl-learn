#include "IndexBuffer.h"

#include "GL/glew.h"
#include "Render.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
    : m_Count(count) {
  Assert(sizeof(unsigned int) == sizeof(GLint));
  GLCall(glGenBuffers(1, &m_RendererID));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int),
                      data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
  GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const {
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const {
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
unsigned int IndexBuffer::GetCount() const {
  return m_Count;
}
