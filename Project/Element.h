#ifndef ELEMENT_H
#define ELEMENT_H

#include <wx/geometry.h>
#include <wx/cursor.h>
#include <wx/menu.h>
#include <GL/gl.h>

enum PickboxID
{
    ID_PB_NONE = 0,
    ID_PB_RIGHT,
    ID_PB_LEFT,
    ID_PB_RIGHT_BOTTOM,
    ID_PB_RIGHT_TOP,
    ID_PB_LEFT_BOTTOM,
    ID_PB_LEFT_TOP
};

enum ContextMenuID
{
    ID_EDIT_BUS = 0,
    ID_EDIT_LINE,
	ID_EDIT_TRANSFORMER,
	ID_EDIT_SYNCGENERATOR,
	ID_EDIT_INDMOTOR,

    ID_LINE_ADD_NODE,
    ID_LINE_REMOVE_NODE,

    ID_ROTATE,
	ID_DELETE
};

class Element
{
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
    // Pure-virtuals methods
    virtual bool AddParent(Element* parent, wxPoint2DDouble position) = 0;
    virtual void Draw(wxPoint2DDouble translation, double scale) const = 0;
    virtual bool Contains(wxPoint2DDouble position) const = 0;
    virtual bool Intersects(wxRect2DDouble rect) const = 0;

    // General methods
	virtual void Rotate() {}
    virtual bool GetContextMenu(wxMenu& menu) { return false; }
    virtual void AddPoint(wxPoint2DDouble point) {}
    virtual void StartMove(wxPoint2DDouble position);
    virtual void Move(wxPoint2DDouble position);
    virtual void MoveNode(Element* parent, wxPoint2DDouble position){}
    virtual bool NodeContains(wxPoint2DDouble position) { return false; }
	virtual void UpdateNodes() {}
	virtual bool SetNodeParent(Element* parent) { return false; }
	virtual void RemoveParent(Element* parent) {}
    virtual void RotateNode(Element* parent) {}
    virtual wxPoint2DDouble GetSwitchPoint(Element* parent,
                                           wxPoint2DDouble parentPoint,
                                           wxPoint2DDouble secondPoint) const;
	virtual bool PickboxContains(wxPoint2DDouble position) { return false;}
    virtual void MovePickbox(wxPoint2DDouble position) {}
    virtual wxCursor GetBestPickboxCursor() const {return wxCURSOR_ARROW;}
    virtual void ResetPickboxes() { m_activePickboxID = ID_PB_NONE; }
	virtual void ResetNodes() { m_activeNodeID= 0; }
    virtual wxPoint2DDouble WorldToScreen(wxPoint2DDouble translation,
                                          double scale,
                                          double offsetX = 0.0,
                                          double offsetY = 0.0) const;
    virtual wxPoint2DDouble WorldToScreen(wxPoint2DDouble position,
                                          wxPoint2DDouble translation,
                                          double scale,
                                          double offsetX = 0.0,
                                          double offsetY = 0.0) const;
	virtual bool RotatedRectanglesIntersects(wxRect2DDouble rect1, wxRect2DDouble rect2, double angle1, double angle2) const;
    virtual void DrawCircle(wxPoint2DDouble position, double radius, int numSegments, GLenum mode = GL_LINE_LOOP) const;
    virtual void DrawRectangle(wxPoint2DDouble position, double width, double height, GLenum mode = GL_QUADS) const;
    virtual void DrawRectangle(wxPoint2DDouble* points, GLenum mode = GL_QUADS) const;
	virtual void DrawPoint(wxPoint2DDouble position, double size) const;
    virtual void DrawLine(std::vector<wxPoint2DDouble> points, GLenum mode = GL_LINE_STRIP) const;
    virtual void DrawPickbox(wxPoint2DDouble position) const;
    virtual wxPoint2DDouble RotateAtPosition(wxPoint2DDouble pointToRotate, double angle, bool degrees = true) const;

    virtual std::vector<Element*> GetParentList() const { return m_parentList; }
    virtual wxPoint2DDouble GetMoveStartPosition() const { return m_moveStartPt; }
    virtual wxPoint2DDouble GetMovePosition() const { return m_movePos; }
   protected:
    std::vector<Element*> m_parentList;

    wxRect2DDouble m_rect;
    wxPoint2DDouble m_position;
    double m_width = 0.0;
    double m_height = 0.0;
    double m_angle = 0.0;
    double m_borderSize = 2.0;
    double m_rotationAngle = 45.0;

    bool m_selected = false;
    bool m_dragging = false;
    bool m_showPickbox = false;

    int m_activePickboxID = ID_PB_NONE;
    int m_activeNodeID = 0;

    wxPoint2DDouble m_moveStartPt;
    wxPoint2DDouble m_movePos;
};

#endif  // ELEMENT_H
