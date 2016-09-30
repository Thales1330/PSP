#ifndef ELEMENT_H
#define ELEMENT_H

#include <wx/msgdlg.h>
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
    ID_EDIT_SYNCMOTOR,
    ID_EDIT_LOAD,
    ID_EDIT_INDUCTOR,
    ID_EDIT_CAPACITOR,

    ID_LINE_ADD_NODE,
    ID_LINE_REMOVE_NODE,

    ID_ROTATE_CLOCK,
	ID_ROTATE_COUNTERCLOCK,
    ID_DELETE
};

enum ElectricalUnit
{
	UNIT_PU = 0,
	UNIT_V,
	UNIT_kV,
	UNIT_A,
	UNIT_kA,
	UNIT_W,
	UNIT_kW,
	UNIT_MW,
	UNIT_VA,
	UNIT_kVA,
	UNIT_MVA,
	UNIT_VAr,
	UNIT_kVAr,
	UNIT_MVAr,
	UNIT_OHM,
	UNIT_OHM_km,
	UNIT_S,
	UNIT_S_km
};

enum FaultData
{
	FAULT_THREEPHASE = 0,
	FAULT_2LINE,
	FAULT_2LINE_GROUND,
	FAULT_LINE_GROUND,
	FAULT_LINE_A,
	FAULT_LINE_B,
	FAULT_LINE_C
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
    void SetOnline(bool online = true);
    // Getters
    wxRect2DDouble GetRect() const { return m_rect; }
    wxPoint2DDouble GetPosition() const { return m_position; }
    bool IsDragging() const { return m_dragging; }
    double GetHeight() const { return m_height; }
    bool IsSelected() const { return m_selected; }
    double GetWidth() const { return m_width; }
    double GetAngle() const { return m_angle; }
    bool IsPickboxShown() const { return m_showPickbox; }
    bool IsOnline() const { return m_online; }
    // Pure-virtuals methods
    virtual bool AddParent(Element* parent, wxPoint2DDouble position) = 0;
    virtual void Draw(wxPoint2DDouble translation, double scale) const = 0;
    virtual bool Contains(wxPoint2DDouble position) const = 0;
    virtual bool Intersects(wxRect2DDouble rect) const = 0;

    // General methods
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
    virtual void RotateNode(Element* parent, bool clockwise = true) {}
    virtual wxPoint2DDouble GetSwitchPoint(Element* parent,
                                           wxPoint2DDouble parentPoint,
                                           wxPoint2DDouble secondPoint) const;
    virtual bool SwitchesContains(wxPoint2DDouble position) const;
    virtual void UpdateSwitches();
    virtual void DrawSwitches() const;

    virtual bool PickboxContains(wxPoint2DDouble position) { return false; }
    virtual void MovePickbox(wxPoint2DDouble position) {}
    virtual wxCursor GetBestPickboxCursor() const { return wxCURSOR_ARROW; }
    virtual void ResetPickboxes() { m_activePickboxID = ID_PB_NONE; }
    virtual void ResetNodes() { m_activeNodeID = 0; }
    virtual wxPoint2DDouble WorldToScreen(wxPoint2DDouble translation,
                                          double scale,
                                          double offsetX = 0.0,
                                          double offsetY = 0.0) const;
    virtual wxPoint2DDouble WorldToScreen(wxPoint2DDouble position,
                                          wxPoint2DDouble translation,
                                          double scale,
                                          double offsetX = 0.0,
                                          double offsetY = 0.0) const;
    virtual bool RotatedRectanglesIntersects(wxRect2DDouble rect1,
                                             wxRect2DDouble rect2,
                                             double angle1,
                                             double angle2) const;

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
    virtual wxPoint2DDouble GetMoveStartPosition() const { return m_moveStartPt; }
    virtual wxPoint2DDouble GetMovePosition() const { return m_movePos; }
	
	virtual void CalculateBoundaries(wxPoint2DDouble& leftUp, wxPoint2DDouble& rightBottom) const;
	
	virtual void GeneralMenuItens(wxMenu& menu);
	
	virtual bool ShowForm(wxWindow* parent, Element* element) { return false; }
	
	bool DoubleFromString(wxWindow* parent, wxString strValue, double& value, wxString errorMsg);
	bool IntFromString(wxWindow* parent, wxString strValue, int& value, wxString errorMsg);
	
	virtual void 
	
   protected:
    std::vector<Element*> m_parentList;

    wxRect2DDouble m_rect;
    wxPoint2DDouble m_position;
    double m_width = 0.0;
    double m_height = 0.0;
    double m_angle = 0.0;
    double m_borderSize = 2.0;
    double m_rotationAngle = 45.0;
    double m_switchSize = 10.0;

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

#endif  // ELEMENT_H
