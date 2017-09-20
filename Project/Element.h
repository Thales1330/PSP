#ifndef ELEMENT_H
#define ELEMENT_H

#include <wx/msgdlg.h>
#include <wx/geometry.h>
#include <wx/cursor.h>
#include <wx/menu.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <GL/gl.h>

#include <complex>

//#include <wx/log.h>

/**
 * @enum PickboxID
 * @brief ID of the pickbox.
*/
enum PickboxID {
    ID_PB_NONE = 0,     /**< No pickbox */
    ID_PB_RIGHT,        /**< Right pickbox */
    ID_PB_LEFT,         /**< Left pickbox */
    ID_PB_RIGHT_BOTTOM, /**< Right-bottom pickbox */
    ID_PB_RIGHT_TOP,    /**< Right-top pickbox */
    ID_PB_LEFT_BOTTOM,  /**< Left-bottom pickbox */
    ID_PB_LEFT_TOP      /**< Left-top pickbox */
};

/**
 * @enum ContextMenuID
 * @brief ID of contex menu itens.
*/
enum ContextMenuID {
    ID_EDIT_ELEMENT = 0,    /**< Edit element */
    ID_LINE_ADD_NODE,       /**< Add node on power lines */
    ID_LINE_REMOVE_NODE,    /**< Remove power line node */
    ID_ROTATE_CLOCK,        /**< Rotate the element clockwise */
    ID_ROTATE_COUNTERCLOCK, /**< Rotate the element counter-clockwise */
    ID_DELETE               /**< Delete the element */
};

/**
 * @class OpenGLColour
 * @author Thales Lima Oliveira
 * @date 18/01/2017
 * @file Element.h
 * @brief Class to manage color of OpenGL.
 */
class OpenGLColour
{
public:
    /**
     * @brief Default construnctor. Use SetRGBA(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha).
     */
    OpenGLColour();

    /**
     * @brief Constructor with RGBA input. The colour values must be between 0.0 and 1.0.
     * @param red Red colour value.
     * @param green Green colour value.
     * @param blue Blue colcour value.
     * @param alpha Apha channel value.
     */
    OpenGLColour(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);

    /**
     * @brief Destructor.
     */
    virtual ~OpenGLColour() {}

    /**
     * @brief Set the colour in RGBA. The colour values must be between 0.0 and 1.0.
     * @param red Red colour value.
     * @param green Green colour value.
     * @param blue Blue colcour value.
     * @param alpha Apha channel value.
     */
    void SetRGBA(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);

    /**
     * @brief Get colour in RGBA.
     * @return RGBA colour.
     */
    const GLdouble* GetRGBA() const { return rgba; }
protected:
    GLdouble rgba[4];
};

/**
 * @class Element
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 19/09/2017
 * @file Element.h
 * @brief Base class of all elements of the program. This class is responsible for manage graphical and his data.
 */
class Element
{
public:
    /**
     * @brief Constructor.
     */
    Element();

    /**
     * @brief Destructor.
     */
    virtual ~Element();

    /**
     * @brief Set if the element are being dragged.
     * @param dragging True if is dragging, false otherwise.
     */
    void SetDragging(bool dragging = true) { m_dragging = dragging; }

    /**
     * @brief Set element height.
     * @param height Height value.
     */
    void SetHeight(double height) { m_height = height; }

    /**
     * @brief Set the element position and update the rectangle.
     * @param position Position value.
     */
    void SetPosition(const wxPoint2DDouble position);

    /**
     * @brief Set element selection
     * @param selected True if selected, false otherwise.
     */
    void SetSelected(bool selected = true) { m_selected = selected; }

    /**
     * @brief Set element width.
     * @param width Width value.
     */
    void SetWidth(double width) { m_width = width; }

    /**
     * @brief Set element angle
     * @param angle Angle value in degrees.
     */
    void SetAngle(double angle) { m_angle = angle; }

    /**
     * @brief Set if the pickbox is shown.
     * @param showPickbox True if show, false otherwise.
     */
    void ShowPickbox(bool showPickbox = true) { m_showPickbox = showPickbox; }

    /**
     * @brief Set the size of the border (shown in selected elements).
     * @param borderSize Border size.
     */
    void SetBorderSize(double borderSize) { m_borderSize = borderSize; }

    /**
     * @brief Set if the element is online or offline.
     * @param online True if online, false if offline.
     */
    bool SetOnline(bool online = true);

    /**
     * @brief Set the list of points that connect the element to the bus.
     * @param pointList List of points.
     */
    virtual void SetPointList(std::vector<wxPoint2DDouble> pointList) { m_pointList = pointList; }

    /**
     * @brief Get the element rectangle.
     * @return Element rectangle.
     */
    wxRect2DDouble GetRect() const { return m_rect; }

    /**
     * @brief Get the element position.
     * @return Element position.
     */
    wxPoint2DDouble GetPosition() const { return m_position; }

    /**
     * @brief Checks if the element is being dragged.
     * @return True if is being dragged, false otherwise.
     */
    bool IsDragging() const { return m_dragging; }

    /**
     * @brief Get the element height.
     * @return Element height.
     */
    double GetHeight() const { return m_height; }

    /**
     * @brief Checks if the element is selected.
     * @return True if selected, false otherwise.
     */
    bool IsSelected() const { return m_selected; }

    /**
     * @brief Get the element width.
     * @return Element width.
     */
    double GetWidth() const { return m_width; }

    /**
     * @brief Get the element angle.
     * @return Angle value.
     */
    double GetAngle() const { return m_angle; }

    /**
     * @brief Get the angle of rotation.
     * @return Rotation angle.
     */
    double GetRotationAngle() const { return m_rotationAngle; }

    /**
     * @brief Checks if the pickbox is shown.
     * @return True if the pickbox is show, false otherwise.
     */
    bool IsPickboxShown() const { return m_showPickbox; }

    /**
     * @brief Checks if the element is online or offline.
     * @return True if online, false if offline.
     */
    bool IsOnline() const { return m_online; }

    /**
     * @brief Get the list of points that connect the element to bus.
     * @return List of points.
     */
    virtual std::vector<wxPoint2DDouble> GetPointList() const { return m_pointList; }

    /**
     * @brief Add a parent to the element. This method must be used on power elements that connect to a bus, so the
     * parent must be a bus.
     * The element basic points are calculated in this method, so apply this when the element is being inserted.
     * @param parent Element parent.
     * @param position Node position in the parent.
     */
    virtual bool AddParent(Element* parent, wxPoint2DDouble position) { return false; }

    /**
     * @brief Add a parent to the element.
     * @param parent Element parent.
     */
    virtual void AddParent(Element* parent) { m_parentList.push_back(parent); }

    /**
     * @brief Checks if the element contains a position.
     * @param position Position to be checked.
     */
    virtual bool Contains(wxPoint2DDouble position) const = 0;

    /**
     * @brief Check if the element's rect intersects other rect.
     * @param rect Rect to check intersection.
     */
    virtual bool Intersects(wxRect2DDouble rect) const = 0;

    /**
     * @brief Get a the element copy.
     * @return Copy of the element.
     */
    virtual Element* GetCopy() { return NULL; }

    /**
     * @brief Set the element ID.
     * @param id Element ID.
     */
    virtual void SetID(int id) { m_elementID = id; }

    /**
     * @brief Get the element ID.
     * @return Element ID.
     */
    virtual int GetID() const { return m_elementID; }

    /**
     * @brief Add a child to the child list.
     * @param child New child.
     */
    virtual void AddChild(Element* child);

    /**
     * @brief Remove a child from the list.
     * @param child Child to remove.
     */
    virtual void RemoveChild(Element* child);

    /**
     * @brief Replace a child from the list.
     * @param oldChild Old child.
     * @param newChild New child.
     */
    virtual void ReplaceChild(Element* oldChild, Element* newChild);

    /**
     * @brief Get the tip text.
     * @return Tip text.
     */
    virtual wxString GetTipText() const { return wxEmptyString; }

    /**
     * @brief Draw the element.
     * @param translation Translation of the system.
     * @param scale Scale of the system.
     */
    virtual void Draw(wxPoint2DDouble translation, double scale) const {}

    /**
     * @brief Rotate the element.
     * @param clockwise True to rotate clockwise, false to rotate counter-clockwise.
     */
    virtual void Rotate(bool clockwise = true) {}

    /**
     * @brief Get the element contex menu.
     * @param menu menu that will be inserted the element itens.
     * @return True if was possible to build the menu, false otherwise.
     */
    virtual bool GetContextMenu(wxMenu& menu) { return false; }

    /**
     * @brief Add point to the list of points that connect the element to the bus.
     * @param point Point to be added.
     */
    virtual void AddPoint(wxPoint2DDouble point) {}

    /**
     * @brief Update the element attributes related to the movement.
     * @param position Start move position.
     */
    virtual void StartMove(wxPoint2DDouble position);

    /**
     * @brief Move the element other position.
     * @param position Position that the element will be moved. Use StartMove(wxPoint2DDouble position) before start
     * moving.
     */
    virtual void Move(wxPoint2DDouble position);

    /**
     * @brief Move a node. StartMove(wxPoint2DDouble position) before start moving.
     * @param parent Node's parent.
     * @param position New node position.
     */
    virtual void MoveNode(Element* parent, wxPoint2DDouble position) {}

    /**
     * @brief Check if a node contains a point. If contains, set the attributes related to node movement.
     * @param position Position tested.
     * @return True if at least one node contains the position, false otherwise.
     */
    virtual bool NodeContains(wxPoint2DDouble position) { return false; }

    /**
     * @brief Update the nodes according to the parents. If a parent is removed, use this method.
     */
    virtual void UpdateNodes() {}

    /**
     * @brief Set a perent to the node. If all conditions are met, a new parent are added to the element and the points
     * related to the nodes will be calculated.
     * @param parent Node parent.
     * @return True if was possible to set the parent.
     */
    virtual bool SetNodeParent(Element* parent) { return false; }

    /**
     * @brief Remove a parent.
     * @param parent Parent to be removed.
     */
    virtual void RemoveParent(Element* parent) {}

    /**
     * @brief Replace a parent.
     * @param oldParent Parent to be replaced.
     * @param newParent New parent.
     */
    virtual void ReplaceParent(Element* oldParent, Element* newParent);

    /**
     * @brief Rotate a node.
     * @param parent Node's parent.
     * @param clockwise True to rotate clockwise, false to rotate counter-clockwise.
     */
    virtual void RotateNode(Element* parent, bool clockwise = true) {}

    /**
     * @brief Check if a pickbox contains a point. If contains the attributes related to pickbox movement will be
     * calculated.
     * @param position Position to be checked.
     * @return True if the element constains the pickbox, false otherwise.
     */
    virtual bool PickboxContains(wxPoint2DDouble position) { return false; }

    /**
     * @brief Move the pickbox.
     * @param position position that the pickbox will be moved.
     */
    virtual void MovePickbox(wxPoint2DDouble position) {}

    /**
     * @brief Get the best cursor to shown to the user when the mouse is above a pickbox.
     * @return Cursor.
     */
    virtual wxCursor GetBestPickboxCursor() const { return wxCURSOR_ARROW; }

    /**
     * @brief Remove the pickboxes.
     */
    virtual void ResetPickboxes() { m_activePickboxID = ID_PB_NONE; }

    /**
     * @brief Remove the active nodes.
     */
    virtual void ResetNodes() { m_activeNodeID = 0; }

    /**
     * @brief Convert the element position to screen position.
     * @param translation System translation.
     * @param scale System scale
     * @param offsetX Offset in X axis.
     * @param offsetY Offset if Y axis.
     */
    virtual wxPoint2DDouble
    WorldToScreen(wxPoint2DDouble translation, double scale, double offsetX = 0.0, double offsetY = 0.0) const;

    /**
     * @brief Convert a generic position to screen position.
     * @param position Position to be converted.
     * @param translation System translation.
     * @param scale System scale.
     * @param offsetX Offset in X axis.
     * @param offsetY Offset in Y axis.
     */
    virtual wxPoint2DDouble WorldToScreen(wxPoint2DDouble position,
        wxPoint2DDouble translation,
        double scale,
        double offsetX = 0.0,
        double offsetY = 0.0) const;

    /**
     * @brief Check if two roteted rectangles intersect.
     * @param rect1 First rect.
     * @param rect2 Second rect.
     * @param angle1 Rotation algle of first rectangle.
     * @param angle2 Rotation angle of second rectangle.
     */
    virtual bool
    RotatedRectanglesIntersects(wxRect2DDouble rect1, wxRect2DDouble rect2, double angle1, double angle2) const;

    /**
     * @brief Draw a circle.
     * @param position Circle position.
     * @param radius Circle radius
     * @param numSegments Num of segments of the circle.
     * @param mode OpenGl primitive.
     */
    virtual void DrawCircle(wxPoint2DDouble position, double radius, int numSegments, GLenum mode = GL_LINE_LOOP) const;
    virtual void DrawArc(wxPoint2DDouble position,
        double radius,
        double initAngle,
        double finalAngle,
        int numSegments,
        GLenum mode = GL_LINE_LOOP) const;

    /**
     * @brief Draw rectangle.
     * @param position Rectangle position.
     * @param width Rectangle width.
     * @param height Rectangle height.
     * @param mode OpenGl primitive.
     */
    virtual void DrawRectangle(wxPoint2DDouble position, double width, double height, GLenum mode = GL_QUADS) const;

    /**
     * @brief Draw rectangle.
     * @param points Rectangle vertices.
     * @param mode OpenGl primitive.
     */
    virtual void DrawRectangle(wxPoint2DDouble* points, GLenum mode = GL_QUADS) const;

    /**
     * @brief Draw a triangle.
     * @param points Triangle vertices.
     * @param mode OpenGl primitive.
     */
    virtual void DrawTriangle(std::vector<wxPoint2DDouble> points, GLenum mode = GL_TRIANGLES) const;

    /**
     * @brief Draw a point.
     * @param position Point position.
     * @param size Point size.
     */
    virtual void DrawPoint(wxPoint2DDouble position, double size) const;

    /**
     * @brief Draw line.
     * @param points Line vertices.
     * @param mode OpenGl primitive.
     */
    virtual void DrawLine(std::vector<wxPoint2DDouble> points, GLenum mode = GL_LINE_STRIP) const;

    /**
     * @brief Draw pickbox.
     * @param position Pickbox position.
     */
    virtual void DrawPickbox(wxPoint2DDouble position) const;

    /**
     * @brief Rotate a point as element position being the origin.
     * @param pointToRotate Point that will be rotated.
     * @param angle Angle of rotation.
     * @param degrees True if the angle is in degrees, false if radians.
     */
    virtual wxPoint2DDouble RotateAtPosition(wxPoint2DDouble pointToRotate, double angle, bool degrees = true) const;

    /**
     * @brief Get the parent list.
     * @return Parent list.
     */
    virtual std::vector<Element*> GetParentList() const { return m_parentList; }

    /**
     * @brief Get the Child list.
     * @return Child List.
     */
    virtual std::vector<Element*> GetChildList() const { return m_childList; }

    // virtual wxPoint2DDouble GetMoveStartPosition() const { return m_moveStartPt; }
    // virtual wxPoint2DDouble GetMovePosition() const { return m_movePos; }

    /**
     * @brief Calculate the element boundaries.
     * @param leftUp Top-left position of the element.
     * @param rightBottom Bottom-right position of the element.
     */
    virtual void CalculateBoundaries(wxPoint2DDouble& leftUp, wxPoint2DDouble& rightBottom) const;

    /**
     * @brief Insert general itens to context menu.
     * @param menu Menu that will be inserted the general itens.
     */
    virtual void GeneralMenuItens(wxMenu& menu);

    /**
     * @brief Show element data form
     * @param parent Form parent
     * @param element Element that will be edited.
     * @return True if the form is shown, false otherwise.
     */
    virtual bool ShowForm(wxWindow* parent, Element* element) { return false; }

    /**
     * @brief Get a double value from a string. Show a error message if the conversion fail.
     * @param parent Message box parent.
     * @param strValue String value to be converted.
     * @param value Double value converted.
     * @param errorMsg Error message.
     */
    static bool DoubleFromString(wxWindow* parent, wxString strValue, double& value, wxString errorMsg);

    /**
     * @brief Convert a string to int. Show a error message if the conversion fail.
     * @param parent Message box parent.
     * @param strValue String value to be converted.
     * @param value Int value converted.
     * @param errorMsg Error message.
     */
    static bool IntFromString(wxWindow* parent, wxString strValue, int& value, wxString errorMsg);

    /**
     * @brief Convert a double value to string.
     * @param value Value to be converted.
     * @param minDecimal Minimum number of decimal places.
     */
    static wxString StringFromDouble(double value, int minDecimal = 1);

    /**
     * @brief Calculate the distance between a line (formed by point list) and a point.
     * @param point origin point.
     * @param segmentNumber Sotores the segment number clicked
     * @return The distance between the point and the line.
     */
    virtual double PointToLineDistance(wxPoint2DDouble point, int* segmentNumber = NULL) const;

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

    OpenGLColour m_selectionColour;
};

#endif // ELEMENT_H
