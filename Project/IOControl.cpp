#include "IOControl.h"
#include "IOControlForm.h"

IOControl::IOControl(int ioFlags, int id) : ControlElement(id)
{
    m_ioFlags = ioFlags;
    
    Node* node = new Node(m_position, Node::NODE_IN, m_borderSize);
    m_nodeList.push_back(node);

    if(ioFlags & IN_TERMINAL_VOLTAGE)
        SetValue(IN_TERMINAL_VOLTAGE);
    else if(ioFlags & IN_VELOCITY)
        SetValue(IN_VELOCITY);
    node->StartMove(m_position);
}

IOControl::~IOControl() {}
void IOControl::Draw(wxPoint2DDouble translation, double scale) const
{
    std::vector<wxPoint2DDouble> pts;
    if(m_angle == 0.0) {
        pts.push_back(m_rect.GetLeftTop() + wxPoint2DDouble(m_borderSize, m_borderSize));
        pts.push_back(m_rect.GetRightTop() + wxPoint2DDouble(-m_borderSize - 10, m_borderSize));
        pts.push_back(m_position + wxPoint2DDouble(m_width / 2 - m_borderSize, 0));
        pts.push_back(m_rect.GetRightBottom() + wxPoint2DDouble(-m_borderSize - 10, -m_borderSize));
        pts.push_back(m_rect.GetLeftBottom() + wxPoint2DDouble(m_borderSize, -m_borderSize));
    } else if(m_angle == 90.0) {
        pts.push_back(m_rect.GetLeftTop() + wxPoint2DDouble(m_borderSize, m_borderSize));
        pts.push_back(m_rect.GetRightTop() + wxPoint2DDouble(-m_borderSize, m_borderSize));
        pts.push_back(m_rect.GetRightBottom() + wxPoint2DDouble(-m_borderSize, -m_borderSize - 10));
        pts.push_back(m_position + wxPoint2DDouble(0, m_height / 2 - m_borderSize));
        pts.push_back(m_rect.GetLeftBottom() + wxPoint2DDouble(m_borderSize, -m_borderSize - 10));
    } else if(m_angle == 180.0) {
        pts.push_back(m_rect.GetLeftTop() + wxPoint2DDouble(m_borderSize + 10, m_borderSize));
        pts.push_back(m_rect.GetRightTop() + wxPoint2DDouble(-m_borderSize, m_borderSize));
        pts.push_back(m_rect.GetRightBottom() + wxPoint2DDouble(-m_borderSize, -m_borderSize));
        pts.push_back(m_rect.GetLeftBottom() + wxPoint2DDouble(m_borderSize + 10, -m_borderSize));
        pts.push_back(m_position + wxPoint2DDouble(-m_width / 2 + m_borderSize, 0));
    } else if(m_angle == 270.0) {
        pts.push_back(m_position + wxPoint2DDouble(0, -m_height / 2 + m_borderSize));
        pts.push_back(m_rect.GetRightTop() + wxPoint2DDouble(-m_borderSize, m_borderSize + 10));
        pts.push_back(m_rect.GetRightBottom() + wxPoint2DDouble(-m_borderSize, -m_borderSize));
        pts.push_back(m_rect.GetLeftBottom() + wxPoint2DDouble(m_borderSize, -m_borderSize));
        pts.push_back(m_rect.GetLeftTop() + wxPoint2DDouble(m_borderSize, m_borderSize + 10));
    }

    if(m_selected) {
        glColor4dv(m_selectionColour.GetRGBA());
        double borderSize = (m_borderSize * 2.0 + 1.0) / scale;
        std::vector<wxPoint2DDouble> selPts = pts;
        if(m_angle == 0.0) {
            selPts[0] += wxPoint2DDouble(-borderSize / 2, -borderSize / 2);
            selPts[1] += wxPoint2DDouble(borderSize / 2, -borderSize / 2);
            selPts[2] += wxPoint2DDouble(1.5 * borderSize / 2, 0);
            selPts[3] += wxPoint2DDouble(borderSize / 2, borderSize / 2);
            selPts[4] += wxPoint2DDouble(-borderSize / 2, borderSize / 2);
        } else if(m_angle == 90.0) {
            selPts[0] += wxPoint2DDouble(-borderSize / 2, -borderSize / 2);
            selPts[1] += wxPoint2DDouble(borderSize / 2, -borderSize / 2);
            selPts[2] += wxPoint2DDouble(borderSize / 2, borderSize / 2);
            selPts[3] += wxPoint2DDouble(0, 1.5 * borderSize / 2);
            selPts[4] += wxPoint2DDouble(-borderSize / 2, borderSize / 2);
        } else if(m_angle == 180.0) {
            selPts[0] += wxPoint2DDouble(-borderSize / 2, -borderSize / 2);
            selPts[1] += wxPoint2DDouble(borderSize / 2, -borderSize / 2);
            selPts[2] += wxPoint2DDouble(borderSize / 2, borderSize / 2);
            selPts[3] += wxPoint2DDouble(-borderSize / 2, borderSize / 2);
            selPts[4] += wxPoint2DDouble(-1.5 * borderSize / 2, 0);
        } else if(m_angle == 270.0) {
            selPts[0] += wxPoint2DDouble(0, -1.5 * borderSize / 2);
            selPts[1] += wxPoint2DDouble(borderSize / 2, -borderSize / 2);
            selPts[2] += wxPoint2DDouble(borderSize / 2, borderSize / 2);
            selPts[3] += wxPoint2DDouble(-borderSize / 2, borderSize / 2);
            selPts[4] += wxPoint2DDouble(-borderSize / 2, -borderSize / 2);
        }
        DrawLine(selPts, GL_POLYGON);
    }
    glLineWidth(1.0);
    glColor4d(1.0, 1.0, 1.0, 1.0);
    DrawLine(pts, GL_POLYGON);
    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawLine(pts, GL_LINE_LOOP);

    // Plot number.
    glEnable(GL_TEXTURE_2D);
    glColor4d(0.0, 0.0, 0.0, 1.0);
    m_glStringValue->bind();
    if(m_angle == 0.0) {
        m_glStringValue->render(m_position.m_x - 5, m_position.m_y);
    } else if(m_angle == 90.0) {
        m_glStringValue->render(m_position.m_x, m_position.m_y - 5);
    } else if(m_angle == 180.0) {
        m_glStringValue->render(m_position.m_x + 5, m_position.m_y);
    } else if(m_angle == 270.0) {
        m_glStringValue->render(m_position.m_x, m_position.m_y + 5);
    }

    glDisable(GL_TEXTURE_2D);

    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawNodes();
}

bool IOControl::ShowForm(wxWindow* parent, Element* element)
{
    IOControlForm* form = new IOControlForm(parent, this);
    if(form->ShowModal() == wxID_OK) {
        form->Destroy();
        return true;
    }
    form->Destroy();
    return false;
}

void IOControl::Rotate(bool clockwise)
{
    if(clockwise)
        m_angle += 90.0;
    else
        m_angle -= 90.0;
    if(m_angle >= 360.0)
        m_angle = 0.0;
    else if(m_angle < 0)
        m_angle = 270.0;

    UpdatePoints();

    for(auto it = m_nodeList.begin(), itEnd = m_nodeList.end(); it != itEnd; ++it) {
        Node* node = *it;
        node->Rotate(clockwise);
    }
}

wxString IOControl::GenerateText()
{
    wxString omega = wxString::FromUTF8("\xCF\x89");

    switch(m_value) {
        case IN_TERMINAL_VOLTAGE: {
            m_ioNodeType = Node::NODE_OUT;
            return _("Vt");
        } break;
        case IN_VELOCITY: {
            m_ioNodeType = Node::NODE_OUT;
            return omega;
        } break;
        case IN_ACTIVE_POWER: {
            m_ioNodeType = Node::NODE_OUT;
            return _("Pe");
        } break;
        case IN_REACTIVE_POWER: {
            m_ioNodeType = Node::NODE_OUT;
            return _("Qe");
        } break;
        case OUT_FIELD_VOLTAGE: {
            m_ioNodeType = Node::NODE_IN;
            return _("Vf");
        } break;
        case OUT_MEC_POWER: {
            m_ioNodeType = Node::NODE_IN;
            return _("Pm");
        } break;
    }
    return "";
}

void IOControl::SetValue(IOFlags value)
{
    m_value = value;
    wxString text = GenerateText();

    wxFont font(m_fontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    wxScreenDC dc;

    if(m_glStringValue) {
        delete m_glStringValue;
        m_glStringValue = NULL;
    }
    m_glStringValue = new wxGLString(text);
    m_glStringValue->setFont(font);
    m_glStringValue->consolidate(&dc);

    m_width = m_glStringValue->getWidth() + 10 + 2 * m_borderSize;
    m_height = m_glStringValue->getheight() + 10 + 2 * m_borderSize;

    SetPosition(m_position);  // Update rectangle.

    UpdatePoints();
}

void IOControl::UpdatePoints()
{
    if(m_nodeList.size() != 0) {
        Node* node = m_nodeList[0];
        if(node->GetNodeType() != m_ioNodeType) {
            // Rotate 180 degrees
            node->Rotate();
            node->Rotate();
        }
        node->SetNodeType(m_ioNodeType);
        if(m_angle == 0.0) {
            if(m_ioNodeType == Node::NODE_IN)
                node->SetPosition(m_position + wxPoint2DDouble(-m_width / 2, 0));
            else
                node->SetPosition(m_position + wxPoint2DDouble(m_width / 2 - 2, 0));
        } else if(m_angle == 90.0) {
            if(m_ioNodeType == Node::NODE_IN)
                node->SetPosition(m_position + wxPoint2DDouble(0, -m_height / 2));
            else
                node->SetPosition(m_position + wxPoint2DDouble(0, m_height / 2 - 2));
        } else if(m_angle == 180.0) {
            if(m_ioNodeType == Node::NODE_IN)
                node->SetPosition(m_position + wxPoint2DDouble(m_width / 2, 0));
            else
                node->SetPosition(m_position + wxPoint2DDouble(2 - m_width / 2, 0));
        } else if(m_angle == 270.0) {
            if(m_ioNodeType == Node::NODE_IN)
                node->SetPosition(m_position + wxPoint2DDouble(0, m_height / 2));
            else
                node->SetPosition(m_position + wxPoint2DDouble(0, 2 - m_height / 2));
        }
    }
}
