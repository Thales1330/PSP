#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include <vector>
#include <GL/glew.h>

#include "Renderer.h"

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case  GL_FLOAT:         return 4;
        case  GL_UNSIGNED_INT:  return 4;
        case  GL_UNSIGNED_BYTE: return 1;
        default: break;
        }
#ifdef _MSC_VER
        ASSERT(false);
#endif
        return(0);
    }
};

class VertexBufferLayout
{
public:
    VertexBufferLayout()
        : m_stride(0) {}
    ~VertexBufferLayout() {}


    template<typename T>
    void Push(unsigned int count)
    {
#ifdef _MSC_VER
        //static_assert(false);
#endif
    }

    inline const std::vector<VertexBufferElement> GetElements() const& { return m_elements; }
    inline unsigned int GetStride() const& { return m_stride; }

private:
    std::vector<VertexBufferElement> m_elements;
    unsigned int m_stride;
};

template<> inline
void VertexBufferLayout::Push<float>(unsigned int count)
{
    m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
    m_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template<> inline
void VertexBufferLayout::Push<unsigned int>(unsigned int count)
{
    m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<> inline
void VertexBufferLayout::Push<unsigned char>(unsigned int count)
{
    m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}

#endif // VERTEXBUFFERLAYOUT_H
