#ifndef ELEMENT_H
#define ELEMENT_H

#include <wx/msgdlg.h>
#include <wx/geometry.h>
#include <wx/cursor.h>
#include <wx/menu.h>
#include <GL/gl.h>

#include <complex>

#include <wx/log.h>

enum PickboxID {
    ID_PB_NONE = 0,
    ID_PB_RIGHT,
    ID_PB_LEFT,
    ID_PB_RIGHT_BOTTOM,
    ID_PB_RIGHT_TOP,
    ID_PB_LEFT_BOTTOM,
    ID_PB_LEFT_TOP
};

enum ContextMenuID {
    ID_EDIT_ELEMENT = 0,

    ID_LINE_ADD_NODE,
    ID_LINE_REMOVE_NODE,

    ID_ROTATE_CLOCK,
    ID_ROTATE_COUNTERCLOCK,
    ID_DELETE
};

class OpenGLColour
{
public:
    OpenGLColour();
    OpenGLColour(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
    virtual ~OpenGLColour() {}
    void SetRGBA(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
    const GLdouble* GetRGBA() const { return rgba; }
protected:
    GLdouble rgba[4];
};

class Element
{
public:
    Element();
    virtual ~Element();

    // Setters
    void SetDragging(bool dragging = true) { m_dragging = dragging; }
    void SetHeight(double height) { m_height = height; }
    void SetPosition(const wxPoint2DDouble position);
    void SetSelected(bool selected = true) { m_selected = selected; }
    void SetWidth(double width) { m_width = width; }
    void SetAngle(double angle) { m_angle = angle; }
    void ShowPickbox(bool showPickbox = true) { m_showPickbox = showPickbox; }
    void SetBorderSize(double borderSize) { m_borderSize = borderSize; }
    void SetOnline(bool online = true);
    virtual void SetPointList(std::vector<wxPoint2DDouble> pointList) { m_pointList = pointList; }
    // Getters
    wxRect2DDouble GetRect() const { return m_rect; }
    wxPoint2DDouble GetPosition() const { return m_position; }
    bool IsDragging() const { return m_dragging; }
    double GetHeight() const { return m_height; }
    bool IsSelected() const { return m_selected; }
    double GetWidth() const { return m_width; }
    double GetAngle() const { return m_angle; }
    double GetRotationAngle() const { return m_rotationAngle; }
    bool IsPickboxShown() const { return m_showPickbox; }
    bool IsOnline() const { return m_online; }
    virtual std::vector<wxPoint2DDouble> GetPointList() const { return m_pointList; }
    // Pure-virtuals methods
    virtual bool AddParent(Element* parent, wxPoint2DDouble position) = 0;
    virtual bool Contains(wxPoint2DDouble position) const = 0;
    virtual bool Intersects(wxRect2DDouble rect) const = 0;

    // General methods
    virtual Element* GetCopy() { return NULL; }
    virtual void SetID(int id) { m_elementID = id; }
    virtual int GetID() const { return m_elementID; }
    virtual void AddChild(Element* child);
    virtual void RemoveChild(Element* child);
    virtual void ReplaceChild(Element* oldChild, Element* newChild);
    virtual wxString GetTipText() const { return wxEmptyString; }
    virtual void Draw(wxPoint2DDouble translation, double scale) const {}
    virtual void Rotate(bool clockwise = true) {}
    virtual bool GetContextMenu(wxMenu& menu) { return false; }
    virtual void AddPoint(wxPoint2DDouble point) {}
    virtual void StartMove(wxPoint2DDouble position);
    virtual void Move(wxPoint2DDouble position);
    virtual void MoveNode(Element* parent, wxPoint2DDouble position) {}
    virtual bool NodeContains(wxPoint2DDouble position) { return false; }
    virtual void UpdateNodes() {}
    virtual bool SetNodeParent(Element* parent) { return false; }
    virtual void RemoveParent(Element* parent) {}
    virtual void ReplaceParent(Element* oldParent, Element* newParent);
    virtual void RotateNode(Element* parent, bool clockwise = true) {}

    virtual bool PickboxContains(wxPoint2DDouble position) { return false; }
    virtual void MovePickbox(wxPoint2DDouble position) {}
    virtual wxCursor GetBestPickboxCursor() const { return wxCURSOR_ARROW; }
    virtual void ResetPickboxes() { m_activePickboxID = ID_PB_NONE; }
    virtual void ResetNodes() { m_activeNodeID = 0; }
    virtual wxPoint2DDouble
    WorldToScreen(wxPoint2DDouble translation, double scale, double offsetX = 0.0, double offsetY = 0.0) const;
    virtual wxPoint2DDouble WorldToScreen(wxPoint2DDouble position,
        wxPoint2DDouble translation,
        double scale,
        double offsetX = 0.0,
        double offsetY = 0.0) const;
    virtual bool
    RotatedRectanglesIntersects(wxRect2DDouble rect1, wxRect2DDouble rect2, double angle1, double angle2) const;

    virtual void DrawCircle(wxPoint2DDouble position, double radius, int numSegments, GLenum mode = GL_LINE_LOOP) const;
    virtual void DrawArc(wxPoint2DDouble position,
        double radius,
        double initAngle,
        double finalAngle,
        int numSegments,
        GLenum mode = GL_LINE_LOOP) const;
    virtual void DrawRectangle(wxPoint2DDouble position, double width, double height, GLenum mode = GL_QUADS) const;
    virtual void DrawRectangle(wxPoint2DDouble* points, GLenum mode = GL_QUADS) const;
    virtual void DrawTriangle(std::vector<wxPoint2DDouble> points, GLenum mode = GL_TRIANGLES) const;
    virtual void DrawPoint(wxPoint2DDouble position, double size) const;
    virtual void DrawLine(std::vector<wxPoint2DDouble> points, GLenum mode = GL_LINE_STRIP) const;
    virtual void DrawPickbox(wxPoint2DDouble position) const;
    virtual wxPoint2DDouble RotateAtPosition(wxPoint2DDouble pointToRotate, double angle, bool degrees = true) const;

    virtual std::vector<Element*> GetParentList() const { return m_parentList; }
    virtual std::vector<Element*> GetChildList() const { return m_childList; }
    virtual wxPoint2DDouble GetMoveStartPosition() const { return m_moveStartPt; }
    virtual wxPoint2DDouble GetMovePosition() const { return m_movePos; }
    virtual void CalculateBoundaries(wxPoint2DDouble& leftUp, wxPoint2DDouble& rightBottom) const;

    virtual void GeneralMenuItens(wxMenu& menu);

    virtual bool ShowForm(wxWindow* parent, Element* element) { return false; }
    bool DoubleFromString(wxWindow* parent, wxString strValue, double& value, wxString errorMsg);
    bool IntFromString(wxWindow* parent, wxString strValue, int& value, wxString errorMsg);
    
    // Static methods
    static wxString StringFromDouble(double value, int minDecimal = 1);

protected:
    int m_elementID = 0;
    std::vector<Element*> m_parentList;
    std::vector<Element*> m_childList;

    wxRect2DDouble m_rect;
    wxPoint2DDouble m_position;
    double m_width = 0.0;
    double m_height = 0.0;
    double m_angle = 0.0;
    double m_borderSize = 2.0;
    double m_rotationAngle = 45.0;
    double m_switchSize = 10.0;

    OpenGLColour m_busColour;
    OpenGLColour m_onlineElementColour;
    OpenGLColour m_offlineElementColour;
    OpenGLColour m_closedSwitchColour;
    OpenGLColour m_openedSwitchColour;
    OpenGLColour m_selectionColour;
    OpenGLColour m_powerFlowArrowColour;

    std::vector<wxRect2DDouble> m_switchRect;

    bool m_selected = false;
    bool m_dragging = false;
    bool m_showPickbox = false;

    int m_activePickboxID = ID_PB_NONE;
    int m_activeNodeID = 0;

    std::vector<wxPoint2DDouble> m_pointList;
    std::vector<wxPoint2DDouble> m_movePts;

    wxPoint2DDouble m_moveStartPt;
    wxPoint2DDouble m_movePos;

    bool m_online = true;
};

#endif // ELEMENT_H
