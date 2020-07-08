#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <GL/glew.h>

class VertexBuffer
{
public:
    //VertexBuffer() {}
    VertexBuffer(const void* data, unsigned int size, unsigned int usage = GL_STATIC_DRAW);
    ~VertexBuffer();

    void SetSubData(const void* data, const unsigned int& size) const;

    void Bind() const;
    void Unbind() const;

protected:
    unsigned int m_rendererID = 0;
};
#endif