#include "Camera.h"

Camera::Camera()
{
    m_translation = wxPoint2DDouble(0, 0);
    m_scale = 1.0;
}

Camera::~Camera() {}
wxPoint2DDouble Camera::ScreenToWorld(wxPoint2DDouble screenCoords) const
{
    return wxPoint2DDouble(
        screenCoords.m_x / m_scale - m_translation.m_x, screenCoords.m_y / m_scale - m_translation.m_y);
}

void Camera::SetTranslation(wxPoint2DDouble screenPoint)
{
    m_translation = screenPoint / m_scale - m_translationStartPt;
}

void Camera::SetScale(wxPoint2DDouble screenPoint, double delta)
{
    m_translation -= screenPoint * (1.0 - m_scale) / m_scale;

    m_scale += delta;

    // Limits: 5% - 300%
    if(m_scale < m_zoomMin) m_scale = m_zoomMin;
    if(m_scale > m_zoomMax) m_scale = m_zoomMax;

    m_translation += screenPoint * (1.0 - m_scale) / m_scale;
}

wxPoint2DDouble Camera::GetMousePosition(bool worldCoords) const
{
    if(worldCoords) return ScreenToWorld(m_mousePosition);
    return m_mousePosition;
}