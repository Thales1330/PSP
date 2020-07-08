#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

class IndexBuffer
{
public:
    //IndexBuffer() {}
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const { return m_count;  }

protected:
    unsigned int m_rendererID = 0;
    unsigned int m_count = 0;
};
#endif