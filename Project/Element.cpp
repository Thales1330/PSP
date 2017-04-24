#include "Element.h"

Element::Element() { m_selectionColour.SetRGBA(0.0, 0.5, 1.0, 0.5); }

Element::~Element() {}

void Element::SetPosition(const wxPoint2DDouble position)
{
    m_position = position;
    m_rect = wxRect2DDouble(m_position.m_x - m_width / 2.0 - m_borderSize,
        m_position.m_y - m_height / 2.0 - m_borderSize, m_width + 2.0 * m_borderSize, m_height + 2.0 * m_borderSize);
}

void Element::DrawCircle(wxPoint2DDouble position, double radius, int numSegments, GLenum mode) const
{
    glBegin(mode);
    for(int i = 0; i < numSegments; i++) {
        double theta = 2.0 * 3.1415926 * double(i) / double(numSegments);
        glVertex2f(radius * std::cos(theta) + position.m_x, radius * std::sin(theta) + position.m_y);
    }
    glEnd();
}

void Element::DrawArc(wxPoint2DDouble position,
    double radius,
    double initAngle,
    double finalAngle,
    int numSegments,
    GLenum mode) const
{
    double initAngRad = wxDegToRad(initAngle);
    double finalAngRad = wxDegToRad(finalAngle);
    glBegin(mode);
    for(int i = 0; i <= numSegments; i++) {
        double theta = initAngRad + (finalAngRad - initAngRad) * double(i) / double(numSegments);
        glVertex2f(radius * std::cos(theta) + position.m_x, radius * std::sin(theta) + position.m_y);
    }
    glEnd();
}

void Element::DrawTriangle(std::vector<wxPoint2DDouble> points, GLenum mode) const
{
    glBegin(mode);
    for(int i = 0; i < 3; i++) {
        glVertex2d(points[i].m_x, points[i].m_y);
    }
    glEnd();
}

void Element::DrawRectangle(wxPoint2DDouble position, double width, double height, GLenum mode) const
{
    glBegin(mode); // TODO: GL_QUADS é obsoleto (OpenGL 3.0+), encontrar outra solução.
    glVertex2d(position.m_x - width / 2.0, position.m_y - height / 2.0);
    glVertex2d(position.m_x - width / 2.0, position.m_y + height / 2.0);
    glVertex2d(position.m_x + width / 2.0, position.m_y + height / 2.0);
    glVertex2d(position.m_x + width / 2.0, position.m_y - height / 2.0);
    glEnd();
}

void Element::DrawRectangle(wxPoint2DDouble* points, GLenum mode) const
{
    glBegin(mode); // TODO: GL_QUADS é obsoleto (OpenGL 3.0+), encontrar outra solução.
    glVertex2d(points[0].m_x, points[0].m_y);
    glVertex2d(points[1].m_x, points[1].m_y);
    glVertex2d(points[2].m_x, points[2].m_y);
    glVertex2d(points[3].m_x, points[3].m_y);
    glEnd();
}

void Element::DrawLine(std::vector<wxPoint2DDouble> points, GLenum mode) const
{
    glBegin(mode);
    for(auto it = points.begin(); it != points.end(); ++it) {
        glVertex2d((*it).m_x, (*it).m_y);
    }
    glEnd();
}

void Element::DrawPickbox(wxPoint2DDouble position) const
{
    glLineWidth(1.0);
    glColor4d(1.0, 1.0, 1.0, 0.8);
    DrawRectangle(position, 8.0, 8.0);
    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawRectangle(position, 8.0, 8.0, GL_LINE_LOOP);
}

wxPoint2DDouble Element::RotateAtPosition(wxPoint2DDouble pointToRotate, double angle, bool degrees) const
{
    double radAngle = angle;
    if(degrees) radAngle = wxDegToRad(angle);
    return wxPoint2DDouble(std::cos(radAngle) * (pointToRotate.m_x - m_position.m_x) -
            std::sin(radAngle) * (pointToRotate.m_y - m_position.m_y) + m_position.m_x,
        std::sin(radAngle) * (pointToRotate.m_x - m_position.m_x) +
            std::cos(radAngle) * (pointToRotate.m_y - m_position.m_y) + m_position.m_y);
}

void Element::StartMove(wxPoint2DDouble position)
{
    this->m_moveStartPt = position;
    this->m_movePos = m_position;
}

void Element::Move(wxPoint2DDouble position) { SetPosition(m_movePos + position - m_moveStartPt); }

wxPoint2DDouble Element::WorldToScreen(wxPoint2DDouble translation, double scale, double offsetX, double offsetY) const
{
    return wxPoint2DDouble(m_position.m_x + offsetX + translation.m_x, m_position.m_y + offsetY + translation.m_y) *
        scale;
}

wxPoint2DDouble Element::WorldToScreen(wxPoint2DDouble position,
    wxPoint2DDouble translation,
    double scale,
    double offsetX,
    double offsetY) const
{
    return wxPoint2DDouble(position.m_x + offsetX + translation.m_x, position.m_y + offsetY + translation.m_y) * scale;
}

void Element::DrawPoint(wxPoint2DDouble position, double size) const
{
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex2d(position.m_x, position.m_y);
    glEnd();
}

bool Element::RotatedRectanglesIntersects(wxRect2DDouble rect1,
    wxRect2DDouble rect2,
    double angle1,
    double angle2) const
{
    wxPoint2DDouble rect1Corners[4] = { rect1.GetLeftTop(), rect1.GetLeftBottom(), rect1.GetRightBottom(),
        rect1.GetRightTop() };
    wxPoint2DDouble rect2Corners[4] = { rect2.GetLeftTop(), rect2.GetLeftBottom(), rect2.GetRightBottom(),
        rect2.GetRightTop() };
    wxPoint2DDouble rect1Center(rect1.m_x + rect1.m_width / 2.0, rect1.m_y + rect1.m_height / 2.0);
    wxPoint2DDouble rect2Center(rect2.m_x + rect2.m_width / 2.0, rect2.m_y + rect2.m_height / 2.0);

    // Rotate the corners.
    double radAngle1 = wxDegToRad(angle1);
    double radAngle2 = wxDegToRad(angle2);

    for(int i = 0; i < 4; i++) {
        rect1Corners[i] = wxPoint2DDouble(std::cos(radAngle1) * (rect1Corners[i].m_x - rect1Center.m_x) -
                std::sin(radAngle1) * (rect1Corners[i].m_y - rect1Center.m_y) + rect1Center.m_x,
            std::sin(radAngle1) * (rect1Corners[i].m_x - rect1Center.m_x) +
                std::cos(radAngle1) * (rect1Corners[i].m_y - rect1Center.m_y) + rect1Center.m_y);

        rect2Corners[i] = wxPoint2DDouble(std::cos(radAngle2) * (rect2Corners[i].m_x - rect2Center.m_x) -
                std::sin(radAngle2) * (rect2Corners[i].m_y - rect2Center.m_y) + rect2Center.m_x,
            std::sin(radAngle2) * (rect2Corners[i].m_x - rect2Center.m_x) +
                std::cos(radAngle2) * (rect2Corners[i].m_y - rect2Center.m_y) + rect2Center.m_y);
    }

    //[Ref] http://www.gamedev.net/page/resources/_/technical/game-programming/2d-rotated-rectangle-collision-r2604

    // Find the rectangles axis to project
    wxPoint2DDouble axis[4] = { rect1Corners[3] - rect1Corners[0], rect1Corners[3] - rect1Corners[2],
        rect2Corners[3] - rect2Corners[0], rect2Corners[3] - rect2Corners[2] };

    // Calculate the projected points to each axis
    wxPoint2DDouble rect1ProjPts[4][4]; // [axis][corner]
    wxPoint2DDouble rect2ProjPts[4][4]; // [axis][corner]
    for(int i = 0; i < 4; i++) {
        double den = axis[i].m_x * axis[i].m_x + axis[i].m_y * axis[i].m_y;
        for(int j = 0; j < 4; j++) {
            double m_rectProj = (rect1Corners[j].m_x * axis[i].m_x + rect1Corners[j].m_y * axis[i].m_y) / den;
            double rectProj = (rect2Corners[j].m_x * axis[i].m_x + rect2Corners[j].m_y * axis[i].m_y) / den;

            rect1ProjPts[i][j] = wxPoint2DDouble(m_rectProj * axis[i].m_x, m_rectProj * axis[i].m_y);
            rect2ProjPts[i][j] = wxPoint2DDouble(rectProj * axis[i].m_x, rectProj * axis[i].m_y);
        }
    }

    // Calculate the scalar value to identify the max and min values on projections
    double rect1Scalar[4][4]; //[axis][corner]
    double rect2Scalar[4][4]; //[axis][corner]
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            rect1Scalar[i][j] = rect1ProjPts[i][j].m_x * axis[i].m_x + rect1ProjPts[i][j].m_y * axis[i].m_y;
            rect2Scalar[i][j] = rect2ProjPts[i][j].m_x * axis[i].m_x + rect2ProjPts[i][j].m_y * axis[i].m_y;
        }
    }
    // Identify the max and min scalar values
    double rect1Min[4];
    double rect1Max[4];
    double rect2Min[4];
    double rect2Max[4];

    for(int i = 0; i < 4; i++) {
        rect1Max[i] = rect1Scalar[i][0];
        rect2Max[i] = rect2Scalar[i][0];
        rect1Min[i] = rect1Scalar[i][0];
        rect2Min[i] = rect2Scalar[i][0];

        for(int j = 1; j < 4; j++) {
            if(rect1Max[i] < rect1Scalar[i][j]) rect1Max[i] = rect1Scalar[i][j];
            if(rect2Max[i] < rect2Scalar[i][j]) rect2Max[i] = rect2Scalar[i][j];

            if(rect1Min[i] > rect1Scalar[i][j]) rect1Min[i] = rect1Scalar[i][j];
            if(rect2Min[i] > rect2Scalar[i][j]) rect2Min[i] = rect2Scalar[i][j];
        }
    }

    // Check if any segment don't overlap
    for(int i = 0; i < 4; i++) {
        if(!(rect2Min[i] <= rect1Max[i] && rect2Max[i] >= rect1Min[i])) return false;
    }

    return true;
}

void Element::SetOnline(bool online)
{
    // Check if any parent is null.
    for(auto it = m_parentList.begin(); it != m_parentList.end(); it++) {
        if(!(*it)) return;
    }
    m_online = online;
}

void Element::GeneralMenuItens(wxMenu& menu)
{
    wxMenuItem* clockItem = new wxMenuItem(&menu, ID_ROTATE_CLOCK, _("Rotate clockwise"));
    clockItem->SetBitmap(wxImage("..\\data\\images\\menu\\rotateClock16.png"));
    menu.Append(clockItem);

    wxMenuItem* counterClockItem = new wxMenuItem(&menu, ID_ROTATE_COUNTERCLOCK, _("Rotate counter-clockwise"));
    counterClockItem->SetBitmap(wxImage("..\\data\\images\\menu\\rotateCounterClock16.png"));
    menu.Append(counterClockItem);

    wxMenuItem* deleteItem = new wxMenuItem(&menu, ID_DELETE, _("Delete"));
    deleteItem->SetBitmap(wxImage("..\\data\\images\\menu\\delete16.png"));
    menu.Append(deleteItem);
}

void Element::CalculateBoundaries(wxPoint2DDouble& leftUp, wxPoint2DDouble& rightBottom) const
{
    // Check rect corners boundaries.

    // Get rectangle corners
    wxPoint2DDouble rectCorner[4] = { m_rect.GetLeftTop(), m_rect.GetLeftBottom(), m_rect.GetRightBottom(),
        m_rect.GetRightTop() };
    // Rotate corners.
    for(int i = 0; i < 4; ++i) {
        rectCorner[i] = RotateAtPosition(rectCorner[i], m_angle);
    }
    leftUp = rectCorner[0];
    rightBottom = rectCorner[0];
    for(int i = 1; i < 4; ++i) {
        if(rectCorner[i].m_x < leftUp.m_x) leftUp.m_x = rectCorner[i].m_x;
        if(rectCorner[i].m_y < leftUp.m_y) leftUp.m_y = rectCorner[i].m_y;
        if(rectCorner[i].m_x > rightBottom.m_x) rightBottom.m_x = rectCorner[i].m_x;
        if(rectCorner[i].m_y > rightBottom.m_y) rightBottom.m_y = rectCorner[i].m_y;
    }

    // Check points list boundaries.
    for(int i = 0; i < (int)m_pointList.size(); i++) {
        if(m_pointList[i].m_x < leftUp.m_x) leftUp.m_x = m_pointList[i].m_x;
        if(m_pointList[i].m_y < leftUp.m_y) leftUp.m_y = m_pointList[i].m_y;
        if(m_pointList[i].m_x > rightBottom.m_x) rightBottom.m_x = m_pointList[i].m_x;
        if(m_pointList[i].m_y > rightBottom.m_y) rightBottom.m_y = m_pointList[i].m_y;
    }
}

bool Element::DoubleFromString(wxWindow* parent, wxString strValue, double& value, wxString errorMsg)
{
    double dValue = 0.0;

    if(!strValue.ToDouble(&dValue)) {
        wxMessageDialog msgDialog(parent, errorMsg, _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
        return false;
    }

    value = dValue;
    return true;
}

bool Element::IntFromString(wxWindow* parent, wxString strValue, int& value, wxString errorMsg)
{
    long int iValue = 0;

    if(!strValue.ToLong(&iValue)) {
        wxMessageDialog msgDialog(parent, errorMsg, _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
        return false;
    }

    value = iValue;
    return true;
}

wxString Element::StringFromDouble(double value, int minDecimal)
{
    wxString str = wxString::FromCDouble(value, 13);
    int cutNumber = 0;
    int numDecimal = 0;
    bool foundCut = false;
    for(int i = (int)str.length() - 1; i >= 0; i--) {
        if(str[i] != '0' && !foundCut) {
            cutNumber = i;
            foundCut = true;
        }
        if(str[i] == '.') {
            numDecimal = i;
            break;
        }
    }

    wxString formatedStr = "";
    if(cutNumber - numDecimal > minDecimal)
        formatedStr = wxString::FromDouble(value, cutNumber - numDecimal);
    else
        formatedStr = wxString::FromDouble(value, minDecimal);

    return formatedStr;
}

void Element::ReplaceParent(Element* oldParent, Element* newParent)
{
    for(int i = 0; i < (int)m_parentList.size(); i++) {
        if(m_parentList[i] == oldParent) m_parentList[i] = newParent;
    }
}

void Element::AddChild(Element* child) { m_childList.push_back(child); }

void Element::RemoveChild(Element* child)
{
    for(auto it = m_childList.begin(); it != m_childList.end(); ++it) {
        Element* element = *it;
        if(element == child) m_childList.erase(it--);
    }
}

void Element::ReplaceChild(Element* oldChild, Element* newChild)
{
    for(int i = 0; i < (int)m_childList.size(); i++) {
        if(m_childList[i] == oldChild) m_childList[i] = newChild;
    }
}

void OpenGLColour::SetRGBA(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)
{
    rgba[0] = red;
    rgba[1] = green;
    rgba[2] = blue;
    rgba[3] = alpha;
}

OpenGLColour::OpenGLColour() { SetRGBA(1.0, 1.0, 1.0, 1.0); }

OpenGLColour::OpenGLColour(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)
{
    SetRGBA(red, green, blue, alpha);
}

double Element::PointToLineDistance(wxPoint2DDouble point, int* segmentNumber) const
{
    //[Ref] http://geomalgorithms.com/a02-_lines.html
    double distance = 100.0; // Big initial distance.
    wxPoint2DDouble p0 = point;

    for(int i = 1; i < (int)m_pointList.size() - 2; i++) {
        double d = 0.0;

        wxPoint2DDouble p1 = m_pointList[i];
        wxPoint2DDouble p2 = m_pointList[i + 1];

        wxPoint2DDouble v = p2 - p1;
        wxPoint2DDouble w = p0 - p1;

        double c1 = w.m_x * v.m_x + w.m_y * v.m_y;
        double c2 = v.m_x * v.m_x + v.m_y * v.m_y;

        if(c1 <= 0.0) {
            d = std::sqrt(std::pow(p0.m_y - p1.m_y, 2) + std::pow(p0.m_x - p1.m_x, 2));
        } else if(c2 <= c1) {
            d = std::sqrt(std::pow(p0.m_y - p2.m_y, 2) + std::pow(p0.m_x - p2.m_x, 2));
        } else {
            d = std::abs((p2.m_y - p1.m_y) * p0.m_x - (p2.m_x - p1.m_x) * p0.m_y + p2.m_x * p1.m_y - p2.m_y * p1.m_x) /
                std::sqrt(std::pow(p2.m_y - p1.m_y, 2) + std::pow(p2.m_x - p1.m_x, 2));
        }
        if(d < distance) {
            distance = d;
            if(segmentNumber) *segmentNumber = i;
        }
    }

    return distance;
}
