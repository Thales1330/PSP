#include "Constant.h"
#include "ConstantForm.h"

Constant::Constant() : ControlElement()
{
    SetValue(m_value);
    m_angle = 180.0;
    Node* nodeOut = new Node(m_position + wxPoint2DDouble(m_width / 2, 0), Node::NODE_OUT, m_borderSize);
    nodeOut->SetAngle(180.0);
    nodeOut->StartMove(m_position);
    m_nodeList.push_back(nodeOut);
}

Constant::~Constant() {}
void Constant::Draw(wxPoint2DDouble translation, double scale) const
{
    glLineWidth(1.0);
    if(m_selected) {
        glColor4dv(m_selectionColour.GetRGBA());
        double borderSize = (m_borderSize * 2.0 + 1.0) / scale;
        DrawRectangle(m_position, m_width + borderSize, m_height + borderSize);
    }
    glColor4d(1.0, 1.0, 1.0, 1.0);
    DrawRectangle(m_position, m_width, m_height);
    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawRectangle(m_position, m_width, m_height, GL_LINE_LOOP);

    // Plot number.
    glEnable(GL_TEXTURE_2D);
    glColor4d(0.0, 0.0, 0.0, 1.0);
    m_glStringValue->bind();
    m_glStringValue->render(m_position.m_x, m_position.m_y);
    glDisable(GL_TEXTURE_2D);

    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawNodes();
}

bool Constant::ShowForm(wxWindow* parent, Element* element)
{
    ConstantForm* form = new ConstantForm(parent, this);
    if(form->ShowModal() == wxID_OK) {
        form->Destroy();
        return true;
    }
    form->Destroy();
    return false;
}

void Constant::Rotate(bool clockwise)
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

void Constant::UpdatePoints()
{
    if(m_nodeList.size() != 0) {
        if(m_angle == 0.0) {
            m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(-m_width / 2, 0));
        } else if(m_angle == 90.0) {
            m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(0, -m_height / 2));
        } else if(m_angle == 180.0) {
            m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(m_width / 2, 0));
        } else if(m_angle == 270.0) {
            m_nodeList[0]->SetPosition(m_position + wxPoint2DDouble(0, m_height / 2));
        }
    }
}

void Constant::SetValue(double value)
{
    m_value = value;
    wxString text = StringFromDouble(m_value);

    wxFont font(m_fontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    wxScreenDC dc;

    if(m_glStringValue) {
        delete m_glStringValue;
        m_glStringValue = NULL;
    }
    m_glStringValue = new wxGLString(text);
    m_glStringValue->setFont(font);
    m_glStringValue->consolidate(&dc);

    m_width = m_glStringValue->getWidth() + 6 + 2 * m_borderSize;
    m_height = m_glStringValue->getheight() + 6 + 2 * m_borderSize;

    UpdatePoints();
}