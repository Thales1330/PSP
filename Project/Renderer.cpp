#include "Renderer.h"
#include <wx/msgdlg.h>
#include <wx/string.h>

#include <fstream>
#include <sstream>
#include <string>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLCheckError()
{
    while (GLenum error = glGetError())
    {
        wxMessageBox(wxString::Format("[OpenGL error (%X)]", error));
        return false;
    }
    return true;
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    // Set shader
    shader.Bind();
    //shader.SetUniform4f("u_Color", colourVec4[0], colourVec4[1], colourVec4[2], colourVec4[3]);

    // Bind vertex array object
    va.Bind();

    // Bind index buffer
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear()
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Ortho2D(float* mat, float left, float right, float bottom, float top)
{
    // this is basically from
    // http://en.wikipedia.org/wiki/Orthographic_projection_(geometry)
    const float zNear = -1.0f;
    const float zFar = 1.0f;
    const float inv_z = 1.0f / (zFar - zNear);
    const float inv_y = 1.0f / (top - bottom);
    const float inv_x = 1.0f / (right - left);

    //first column
    *mat++ = (2.0f * inv_x);
    *mat++ = (0.0f);
    *mat++ = (0.0f);
    *mat++ = (0.0f);

    //second
    *mat++ = (0.0f);
    *mat++ = (2.0 * inv_y);
    *mat++ = (0.0f);
    *mat++ = (0.0f);

    //third
    *mat++ = (0.0f);
    *mat++ = (0.0f);
    *mat++ = (-2.0f * inv_z);
    *mat++ = (0.0f);

    //fourth
    *mat++ = (-(right + left) * inv_x);
    *mat++ = (-(top + bottom) * inv_y);
    *mat++ = (-(zFar + zNear) * inv_z);
    *mat++ = (1.0f);
}
