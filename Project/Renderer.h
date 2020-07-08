#pragma once

#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#ifdef _MSC_VER
#define  ASSERT(x) if (!(x)) __debugbreak();
#endif //_MSC_VER
#define GLCall(x) GLClearError();\
    x;\
    wxASSERT(GLCheckError())

void GLClearError();
bool GLCheckError();

class Renderer
{
public:
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void Clear();
    void Ortho2D(float* mat, float left, float right, float bottom, float top);
};