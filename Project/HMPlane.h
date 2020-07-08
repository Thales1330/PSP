#ifndef HMPLANE_H
#define HMPLANE_H

#include <vector>
#include "glm/gtc/matrix_transform.hpp"
#include <wx/geometry.h>

class VertexBuffer;
class IndexBuffer;
class VertexBufferLayout;
class VertexArray;

class OpenGLUtils;
class Shader;
class Renderer;

class OpenGLText;

struct BufferMeshCoords
{
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
};

class HMPlane
{
public:
    HMPlane(Shader* shader, Shader* labelShader, const float& width, const float& height, const float limits[2]);
    virtual ~HMPlane();

    virtual void Draw(const Renderer& renderer, const glm::mat4& projectionViewMatrix) const;
    virtual void DrawLabel(const Renderer& renderer, const glm::mat4& projectionViewMatrix, const float& x = 0.0, const float& y = 0.0) const;

    virtual void SetLabelLimits(const float& min, const float& max);
    virtual float GetMaxLimit() { return m_limits[0]; }
    virtual float GetMinLimit() { return m_limits[1]; }

    virtual void SetRectSlope(const wxRect2DDouble& rect, const float& angle, const float& depth);
    virtual void UpdateCoords() { FillIndexBuffer(); }
    virtual void Resize(const float& width, const float& height);

    virtual void SmoothPlane(const unsigned int& iterations);
    virtual void Clear();

protected:

    void FillCoordsBuffer();
    void FillIndexBuffer();

    void BindOpenGLBuffers();

    void CreateLabel();

    const float m_meshSize = 15.0f;
    unsigned int m_meshTickX = 0;
    unsigned int m_meshTickY = 0;
    float m_width = 0.0;
    float m_height = 0.0;

    std::vector< std::vector<BufferMeshCoords*> > m_coords;
    std::vector<float> m_bufferCoords;
    std::vector<unsigned int> m_indexBuffer;

    // OpenGL shader
    Shader* m_shader = nullptr;
    Shader* m_labelShader = nullptr;

    // Buffers
    VertexBuffer* m_vb = nullptr;
    VertexBufferLayout* m_layout = nullptr;
    IndexBuffer* m_ib = nullptr;
    VertexArray* m_va = nullptr;

    // Label
    float m_vertexBufferLabel[4 * 3];
    unsigned int m_indexBufferLabel[6];

    VertexBuffer* m_vbL = nullptr;
    VertexBufferLayout* m_layoutL = nullptr;
    IndexBuffer* m_ibL = nullptr;
    VertexArray* m_vaL = nullptr;

    std::vector<OpenGLText*> m_glTexts;
    float m_limits[2] = {1.1, 0.9};

    bool m_isClear = true;
};

#endif
