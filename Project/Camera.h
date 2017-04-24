#ifndef CAMERA_H
#define CAMERA_H

#include <wx/geometry.h>

class Camera
{
public:
    Camera();
    ~Camera();

    void SetScale(wxPoint2DDouble screenPoint, double delta);
    void SetScale(double scale) { m_scale = scale; }
    void SetTranslation(wxPoint2DDouble screenPoint);
    void StartTranslation(wxPoint2DDouble startPoint) { this->m_translationStartPt = startPoint; }
    void UpdateMousePosition(wxPoint2DDouble mousePosition) { this->m_mousePosition = mousePosition; }
    double GetScale() const { return m_scale; }
    wxPoint2DDouble GetTranslation() const { return m_translation; }
    wxPoint2DDouble GetMousePosition(bool worldCoords = true) const;
    wxPoint2DDouble ScreenToWorld(wxPoint2DDouble screenCoords) const;
    double GetZoomMin() const { return m_zoomMin; }
    double GetZoomMax() const { return m_zoomMax; }

protected:
    wxPoint2DDouble m_translation;
    wxPoint2DDouble m_translationStartPt;
    double m_scale;

    wxPoint2DDouble m_mousePosition;

    double m_zoomMin = 0.01;
    double m_zoomMax = 3.0;
};

#endif // CAMERA_H
