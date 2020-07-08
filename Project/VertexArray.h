#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "VertexBuffer.h"
class VertexBufferLayout;

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void UpdateBuffer(const VertexBuffer& vb, const void* newVBData, const unsigned int& newVBSize);

    void Bind() const;
    void Unbind() const;
private:
    unsigned int m_rendererID;
};


#endif // VERTEXARRAY_H
