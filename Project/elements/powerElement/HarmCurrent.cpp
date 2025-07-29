#include "HarmCurrent.h"

#include "../../forms/HarmCurrentForm.h"

HarmCurrent::HarmCurrent() : Shunt() {}

HarmCurrent::~HarmCurrent() {}

HarmCurrent::HarmCurrent(wxString name) : Shunt() { m_electricalData.name = name; }

Element* HarmCurrent::GetCopy()
{
    HarmCurrent* copy = new HarmCurrent();
    *copy = *this;
    return copy;
}

bool HarmCurrent::AddParent(Element* parent, wxPoint2DDouble position)
{
    if(parent) {
        m_parentList.push_back(parent);
        parent->AddChild(this);
        wxPoint2DDouble parentPt =
            parent->RotateAtPosition(position, -parent->GetAngle());        // Rotate click to horizontal position.
        parentPt.m_y = parent->GetPosition().m_y;                           // Centralize on bus.
        parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());  // Rotate back.

        m_position = parentPt + wxPoint2DDouble(0.0, 100.0);  // Shifts the position to the down of the bus.
        m_width = 40;
        m_height = 60;
        m_rect = wxRect2DDouble(m_position.m_x - m_width / 2.0, m_position.m_y - m_height / 2.0, m_width, m_height);

        m_pointList.push_back(parentPt);
        m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_position));
        m_pointList.push_back(m_position + wxPoint2DDouble(0.0, -m_height / 2.0 - 10.0));
        m_pointList.push_back(m_position + wxPoint2DDouble(0.0, -m_height / 2.0));

        m_triangPts.push_back(wxPoint2DDouble(-5, -15));
        m_triangPts.push_back(wxPoint2DDouble(5, -15));
        m_triangPts.push_back(wxPoint2DDouble(0.0, -30.0));

        m_inserted = true;

        wxRect2DDouble genRect(0, 0, 0, 0);
        m_switchRect.push_back(genRect);  // Push a general rectangle.
        UpdateSwitches();

        return true;
    }
    return false;
}

//void HarmCurrent::Draw(wxPoint2DDouble translation, double scale) const
//{
//    OpenGLColour elementColour;
//    if(m_online) {
//        if(m_dynEvent)
//            elementColour = m_dynamicEventColour;
//        else
//            elementColour = m_onlineElementColour;
//    } else
//        elementColour = m_offlineElementColour;
//
//    if(m_inserted) {
//        std::vector<wxPoint2DDouble> arrowPts;
//        arrowPts.push_back(wxPoint2DDouble(m_position.m_x, m_position.m_y - m_height / 2.0));
//        arrowPts.push_back(arrowPts[0] + wxPoint2DDouble(0, 40));
//
//        if(m_selected) {
//            glLineWidth(1.5 + m_borderSize * 2.0);
//            glColor4dv(m_selectionColour.GetRGBA());
//
//            DrawLine(m_pointList);
//
//            glPushMatrix();
//            glTranslated(m_position.m_x, m_position.m_y, 0.0);
//            glRotated(m_angle, 0.0, 0.0, 1.0);
//            glTranslated(-m_position.m_x, -m_position.m_y, 0.0);
//
//            DrawCircle(wxPoint2DDouble(m_position.m_x, m_position.m_y - m_height / 2.0 + 20),
//                       20.0 + (m_borderSize + 1.5) / scale, 20, GL_POLYGON);
//
//            DrawGround(m_position + wxPoint2DDouble(0, 10.0));
//
//            glPopMatrix();
//
//            // Draw node selection.
//            DrawCircle(m_pointList[0], 5.0 + m_borderSize / scale, 10, GL_POLYGON);
//        }
//        // Draw Harmonic current source (layer 2).
//        glLineWidth(1.5);
//        glColor4dv(elementColour.GetRGBA());
//        DrawCircle(m_pointList[0], 5.0, 10, GL_POLYGON);
//        DrawLine(m_pointList);
//
//        DrawSwitches();
//
//        glPushMatrix();
//        glTranslated(m_position.m_x, m_position.m_y, 0.0);
//        glRotated(m_angle, 0.0, 0.0, 1.0);
//        glTranslated(-m_position.m_x, -m_position.m_y, 0.0);
//
//        std::vector<wxPoint2DDouble> triangPts;
//        for(int i = 0; i < 3; i++) { triangPts.push_back(m_triangPts[i] + m_position); }
//
//        glColor4dv(elementColour.GetRGBA());
//        glColor4d(1.0, 1.0, 1.0, 1.0);
//        DrawCircle(wxPoint2DDouble(m_position.m_x, m_position.m_y - m_height / 2.0 + 20), 20, 20, GL_POLYGON);
//
//        glColor4dv(elementColour.GetRGBA());
//        DrawCircle(wxPoint2DDouble(m_position.m_x, m_position.m_y - m_height / 2.0 + 20), 20, 20);
//        DrawTriangle(triangPts);
//        DrawLine(arrowPts);
//        DrawGround(m_position + wxPoint2DDouble(0, 10.0));
//
//        glPopMatrix();
//    }
//}

void HarmCurrent::DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const
{
    wxColour elementColour;
    if (m_online) {
        if (m_dynEvent)
            elementColour = m_dynamicEventColour;
        else
            elementColour = m_onlineElementColour;
    }
    else
        elementColour = m_offlineElementColour;

    if (m_inserted) {
        std::vector<wxPoint2DDouble> arrowPts;
        arrowPts.push_back(wxPoint2DDouble(m_position.m_x, m_position.m_y - m_height / 2.0));
        arrowPts.push_back(arrowPts[0] + wxPoint2DDouble(0, 40));

        // Draw Selection (layer 1).
        if (m_selected) {
            gc->SetPen(wxPen(wxColour(m_selectionColour), 2 + m_borderSize * 2.0));
            gc->SetBrush(*wxTRANSPARENT_BRUSH);

            gc->StrokeLines(m_pointList.size(), &m_pointList[0]);
            DrawDCGround(m_position + wxPoint2DDouble(0, 10.0), gc);

            gc->SetPen(*wxTRANSPARENT_PEN);
            gc->SetBrush(wxBrush(wxColour(m_selectionColour)));

            // Push the current matrix on stack.
            gc->PushState();
            // Rotate the matrix around the object position.
            gc->Translate(m_position.m_x, m_position.m_y);
            gc->Rotate(wxDegToRad(m_angle));
            gc->Translate(-m_position.m_x, -m_position.m_y);

            DrawDCCircle(wxPoint2DDouble(m_position.m_x, m_position.m_y - m_height / 2.0 + 20),
                20.0 + (m_borderSize + 1.5) / scale, 20, gc);

            gc->PopState();

            // Draw node selection.
            DrawDCCircle(m_pointList[0], 5.0 + m_borderSize / scale, 10, gc);
        }

        // Draw Harmonic current source (layer 2).
        gc->SetPen(wxPen(wxColour(elementColour), 2));
        gc->SetBrush(*wxTRANSPARENT_BRUSH);
        gc->StrokeLines(m_pointList.size(), &m_pointList[0]);

        // Draw node.
        gc->SetPen(*wxTRANSPARENT_PEN);
        gc->SetBrush(wxBrush(wxColour(elementColour)));
        DrawDCCircle(m_pointList[0], 5.0, 10, gc);

        DrawDCSwitches(gc);

        std::vector<wxPoint2DDouble> triangPts;
        for (int i = 0; i < 3; i++) { triangPts.push_back(m_triangPts[i] + m_position); }

        //glColor4dv(elementColour);
        gc->SetPen(wxPen(wxColour(elementColour), 2));
        gc->SetBrush(*wxWHITE_BRUSH);
        DrawDCCircle(wxPoint2DDouble(m_position.m_x, m_position.m_y - m_height / 2.0 + 20), 20, 20, gc);

        gc->PushState();
        gc->Translate(m_position.m_x, m_position.m_y);
        gc->Rotate(wxDegToRad(m_angle));
        gc->Translate(-m_position.m_x, -m_position.m_y);
        
        gc->SetPen(*wxTRANSPARENT_PEN);
        gc->SetBrush(wxBrush(wxColour(elementColour)));

        DrawDCTriangle(triangPts, gc);

        gc->SetPen(wxPen(wxColour(elementColour), 2));
        gc->StrokeLines(arrowPts.size(), &arrowPts[0]);
        DrawDCGround(m_position + wxPoint2DDouble(0, 10.0), gc);

        gc->PopState();
    }
}

bool HarmCurrent::Contains(wxPoint2DDouble position) const
{
    wxPoint2DDouble ptR = RotateAtPosition(position, -m_angle);
    return m_rect.Contains(ptR);
}

bool HarmCurrent::Intersects(wxRect2DDouble rect) const
{
    return RotatedRectanglesIntersects(m_rect, rect, m_angle, 0.0);
}

void HarmCurrent::Rotate(bool clockwise)
{
    double rotAngle = m_rotationAngle;
    if(!clockwise) rotAngle = -m_rotationAngle;

    m_angle += rotAngle;
    if(m_angle >= 360 || m_angle <= -360) m_angle = 0.0;
    m_pointList[2] = RotateAtPosition(m_pointList[2], rotAngle);
    m_pointList[3] = RotateAtPosition(m_pointList[3], rotAngle);
    UpdateSwitchesPosition();
}

bool HarmCurrent::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_ELEMENT, _("Edit Harmonic Current Source"));
    GeneralMenuItens(menu);
    return true;
}

wxString HarmCurrent::GetTipText() const
{
    wxString tipText = m_electricalData.name;

    for(unsigned int i = 0; i < m_electricalData.harmonicOrder.size(); ++i) {
        tipText +=
            wxString::Format("\nI%dh = %s %s (%s%s)", m_electricalData.harmonicOrder[i],
                             StringFromDouble(m_electricalData.injHarmCurrent[i]),
                             m_electricalData.injHarmCurrentUnit[i] == ElectricalUnit::UNIT_A ? "A" : "p.u.",
                             StringFromDouble(m_electricalData.injHarmAngle[i]), static_cast<wxString>(L'\u00B0'));
    }

    return tipText;
}

bool HarmCurrent::ShowForm(wxWindow* parent, Element* element)
{
    HarmCurrentForm* harmCurrentForm = new HarmCurrentForm(parent, this);
    harmCurrentForm->SetTitle(_("Harmonic Current Source"));
    if(harmCurrentForm->ShowModal() == wxID_OK) {
        harmCurrentForm->Destroy();
        return true;
    }
    harmCurrentForm->Destroy();
    return false;
}

HarmCurrentElectricalData HarmCurrent::GetPUElectricalData(double systemPowerBase, double voltage)
{
    HarmCurrentElectricalData puData = m_electricalData;
    double ib = systemPowerBase / (std::sqrt(3.00) * voltage);
    for(unsigned int i = 0; i < puData.harmonicOrder.size(); ++i) {
        if(puData.injHarmCurrentUnit[i] == ElectricalUnit::UNIT_A) {
            puData.injHarmCurrent[i] /= ib;
            puData.injHarmCurrentUnit[i] = ElectricalUnit::UNIT_PU;
        }
    }
    return puData;
}

rapidxml::xml_node<>* HarmCurrent::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
    auto elementNode = XMLParser::AppendNode(doc, elementListNode, "HarmCurrent");
    XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);

    SaveCADProperties(doc, elementNode);

    auto electricalProp = XMLParser::AppendNode(doc, elementNode, "ElectricalProperties");
    auto isOnline = XMLParser::AppendNode(doc, electricalProp, "IsOnline");
    XMLParser::SetNodeValue(doc, isOnline, m_online);
    auto name = XMLParser::AppendNode(doc, electricalProp, "Name");
    XMLParser::SetNodeValue(doc, name, m_electricalData.name);

    auto harmCurrentDataList = XMLParser::AppendNode(doc, electricalProp, "HarmCurrentList");
    for(int i = 0; i < static_cast<int>(m_electricalData.harmonicOrder.size()); i++) {
        auto harmCurrentData = XMLParser::AppendNode(doc, harmCurrentDataList, "HarmCurrent");
        XMLParser::SetNodeAttribute(doc, harmCurrentData, "ID", i);
        auto order = XMLParser::AppendNode(doc, harmCurrentData, "Order");
        XMLParser::SetNodeValue(doc, order, m_electricalData.harmonicOrder[i]);
        auto injCurrent = XMLParser::AppendNode(doc, harmCurrentData, "InjCurrent");
        XMLParser::SetNodeValue(doc, injCurrent, m_electricalData.injHarmCurrent[i]);
        XMLParser::SetNodeAttribute(doc, injCurrent, "UnitID", static_cast<int>(m_electricalData.injHarmCurrentUnit[i]));
        auto injHarmAngle = XMLParser::AppendNode(doc, harmCurrentData, "Angle");
        XMLParser::SetNodeValue(doc, injHarmAngle, m_electricalData.injHarmAngle[i]);
    }

    return elementNode;
}

bool HarmCurrent::OpenElement(rapidxml::xml_node<>* elementNode, std::vector<Element*> parentList)
{
    if(!OpenCADProperties(elementNode, parentList)) return false;

    auto electricalProp = elementNode->first_node("ElectricalProperties");
    if(!electricalProp) return false;

    SetOnline(XMLParser::GetNodeValueInt(electricalProp, "IsOnline"));
    m_electricalData.name = electricalProp->first_node("Name")->value();

    auto harmCurrentList = electricalProp->first_node("HarmCurrentList");
    auto harmCurrent = harmCurrentList->first_node("HarmCurrent");
    while(harmCurrent) {
        m_electricalData.harmonicOrder.push_back(XMLParser::GetNodeValueInt(harmCurrent, "Order"));
        m_electricalData.injHarmCurrent.push_back(XMLParser::GetNodeValueDouble(harmCurrent, "InjCurrent"));
        m_electricalData.injHarmCurrentUnit.push_back(
            static_cast<ElectricalUnit>(XMLParser::GetAttributeValueInt(harmCurrent, "InjCurrent", "UnitID")));
        m_electricalData.injHarmAngle.push_back(XMLParser::GetNodeValueDouble(harmCurrent, "Angle"));
        harmCurrent = harmCurrent->next_sibling("HarmCurrent");
    }

    m_inserted = true;
    return true;
}
