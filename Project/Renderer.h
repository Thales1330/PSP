#pragma once

#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define  ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLCheckError())

void GLClearError();
bool GLCheckError();

class Renderer
{
public:
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void Clear();
    void Ortho2D(float* mat, float left, float right, float bottom, float top);
};