#ifndef ELEMENT_H
#define ELEMENT_H

#include <wx/geometry.h>
#include <wx/cursor.h>
#include <GL/gl.h>

class Element
{
   protected:
    wxRect2DDouble m_rect;
    wxPoint2DDouble m_position;
    double m_width = 0.0;
    double m_height = 0.0;
    double m_angle = 0.0;
    double m_borderSize = 2.0;

    bool m_selected = false;
    bool m_dragging = false;
    bool m_showPickbox = false;

   public:
    Element();
    virtual ~Element();

    // Setters
    void SetDragging(bool dragging = true) { this->m_dragging = dragging; }
    void SetHeight(double height) { this->m_height = height; }
    void SetPosition(const wxPoint2DDouble position);
    void SetSelected(bool selected = true) { this->m_selected = selected; }
    void SetWidth(double width) { this->m_width = width; }
    void SetAngle(double angle) { this->m_angle = angle; }
    void ShowPickbox(bool showPickbox = true) { this->m_showPickbox = showPickbox; }
    void SetBorderSize(double borderSize) { this->m_borderSize = borderSize; }
    // Getters
    wxRect2DDouble GetRect() const { return m_rect; }
    wxPoint2DDouble GetPosition() const { return m_position; }
    bool IsDragging() const { return m_dragging; }
    double GetHeight() const { return m_height; }
    bool IsSelected() const { return m_selected; }
    double GetWidth() const { return m_width; }
    double GetAngle() const { return m_angle; }
    bool IsPickboxShown() const { return m_showPickbox; }
    // Métodos virtuais
    virtual void Draw(wxPoint2DDouble translation, double scale) const = 0;
    virtual bool Contains(wxPoint2DDouble position) const = 0;
    virtual int PickboxContains(wxPoint2DDouble position) const = 0;
    virtual void MovePickbox(wxPoint2DDouble position, int pickboxID) = 0;
    virtual wxCursor GetBestPickboxCursor() const = 0;

    // Métodos gerais
    wxPoint2DDouble WorldToScreen(wxPoint2DDouble translation,
                                  double scale,
                                  double offsetX = 0.0,
                                  double offsetY = 0.0) const;
    void DrawCircle(wxPoint2DDouble position, double radius, int numSegments, GLenum mode = GL_LINE_LOOP) const;
    void DrawRectangle(wxPoint2DDouble position, double width, double height, GLenum mode = GL_QUADS) const;
};

#endif  // ELEMENT_H
