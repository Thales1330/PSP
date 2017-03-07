#include "Sum.h"
#include "SumForm.h"

Sum::Sum()
    : ControlElement()
{
    m_width = m_height = 36.0;
    Node* nodeIn1 = new Node(m_position + wxPoint2DDouble(-m_width / 2, 9 - m_height / 2), Node::NODE_IN, m_borderSize);
    nodeIn1->StartMove(m_position);
    Node* nodeIn2 =
        new Node(m_position + wxPoint2DDouble(-m_width / 2, 27 - m_height / 2), Node::NODE_IN, m_borderSize);
    nodeIn2->StartMove(m_position);
    Node* nodeOut = new Node(m_position + wxPoint2DDouble(m_width / 2, 0), Node::NODE_OUT, m_borderSize);
    nodeOut->SetAngle(180.0);
    nodeOut->StartMove(m_position);
    m_nodeList.push_back(nodeIn1);
    m_nodeList.push_back(nodeIn2);
    m_nodeList.push_back(nodeOut);
    m_signalList.push_back(SIGNAL_POSITIVE);
    m_signalList.push_back(SIGNAL_NEGATIVE);
}

Sum::~Sum() {}

void Sum::Draw(wxPoint2DDouble translation, double scale) const
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

    // Plot signals.
    glLineWidth(2.0);
    for(int i = 0; i < (int)m_nodeList.size() - 1; ++i) {
        std::vector<wxPoint2DDouble> hLine;
        hLine.push_back(m_nodeList[i]->GetPosition() + wxPoint2DDouble(6, 0));
        hLine.push_back(m_nodeList[i]->GetPosition() + wxPoint2DDouble(12, 0));
        DrawLine(hLine);
        if(m_signalList[i] == SIGNAL_POSITIVE) {
            std::vector<wxPoint2DDouble> vLine;
            vLine.push_back(m_nodeList[i]->GetPosition() + wxPoint2DDouble(9, -3));
            vLine.push_back(m_nodeList[i]->GetPosition() + wxPoint2DDouble(9, 3));
            DrawLine(vLine);
        }
    }
    
    //Plot sigma.
    std::vector<wxPoint2DDouble> sigma;
    sigma.push_back(m_position + wxPoint2DDouble(10, 9));
    sigma.push_back(m_position + wxPoint2DDouble(0, 9));
    sigma.push_back(m_position + wxPoint2DDouble(6, 0));
    sigma.push_back(m_position + wxPoint2DDouble(0, -9));
    sigma.push_back(m_position + wxPoint2DDouble(10, -9));
    glColor4d(0.0, 0.3, 1.0, 1.0);
    DrawLine(sigma);
    
    glColor4d(0.0, 0.0, 0.0, 1.0);
    DrawNodes();
}

bool Sum::ShowForm(wxWindow* parent, Element* element)
{
    SumForm* sumForm = new SumForm(parent, this);
    if(sumForm->ShowModal() == wxID_OK) {
        sumForm->Destroy();
        return true;
    }
    sumForm->Destroy();
    return false;
}
