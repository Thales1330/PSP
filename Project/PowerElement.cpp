#include "PowerElement.h"
#ifdef USING_WX_3_0_X
#include "DegreesAndRadians.h"
#endif

PowerElement::PowerElement() : Element()
{
    m_busColour.SetRGBA(0.0, 0.3, 1.0, 1.0);
    m_onlineElementColour.SetRGBA(0.2, 0.2, 0.2, 1.0);
    m_offlineElementColour.SetRGBA(0.5, 0.5, 0.5, 1.0);
    m_closedSwitchColour.SetRGBA(0.0, 0.4, 0.0, 1.0);
    m_openedSwitchColour.SetRGBA(1.0, 0.1, 0.1, 1.0);
    m_powerFlowArrowColour.SetRGBA(1.0, 0.51, 0.0, 1.0);
    m_dynamicEventColour.SetRGBA(1.0, 0.51, 0.0, 1.0);
}

PowerElement::~PowerElement() {}
void PowerElement::SetNominalVoltage(std::vector<double> nominalVoltage, std::vector<ElectricalUnit> nominalVoltageUnit)
{
}

wxPoint2DDouble PowerElement::GetSwitchPoint(Element* parent,
                                             wxPoint2DDouble parentPoint,
                                             wxPoint2DDouble secondPoint) const
{
    double swLineSize = 25.0;
    wxPoint2DDouble swPoint = wxPoint2DDouble(parentPoint.m_x, parentPoint.m_y - swLineSize);

    // Rotate the second point (to compare).
    double angle = parent->GetAngle();

    secondPoint =
        wxPoint2DDouble(std::cos(wxDegToRad(-angle)) * (secondPoint.m_x - parentPoint.m_x) -
                            std::sin(wxDegToRad(-angle)) * (secondPoint.m_y - parentPoint.m_y) + parentPoint.m_x,
                        std::sin(wxDegToRad(-angle)) * (secondPoint.m_x - parentPoint.m_x) +
                            std::cos(wxDegToRad(-angle)) * (secondPoint.m_y - parentPoint.m_y) + parentPoint.m_y);

    // Rotate
    if(secondPoint.m_y > parentPoint.m_y) angle -= 180.0;
    return wxPoint2DDouble(std::cos(wxDegToRad(angle)) * (swPoint.m_x - parentPoint.m_x) -
                               std::sin(wxDegToRad(angle)) * (swPoint.m_y - parentPoint.m_y) + parentPoint.m_x,
                           std::sin(wxDegToRad(angle)) * (swPoint.m_x - parentPoint.m_x) +
                               std::cos(wxDegToRad(angle)) * (swPoint.m_y - parentPoint.m_y) + parentPoint.m_y);
}

bool PowerElement::SwitchesContains(wxPoint2DDouble position) const
{
    for(int i = 0; i < (int)m_switchRect.size(); i++) {
        if(m_parentList[i]) {
            if(m_switchRect[i].Contains(position)) return true;
        }
    }
    return false;
}

void PowerElement::UpdateSwitches()
{
    // General method, to one switch only.
    wxPoint2DDouble swCenter = wxPoint2DDouble((m_pointList[0].m_x + m_pointList[1].m_x) / 2.0,
                                               (m_pointList[0].m_y + m_pointList[1].m_y) / 2.0);
    m_switchRect[0] = wxRect2DDouble(swCenter.m_x - m_switchSize / 2.0, swCenter.m_y - m_switchSize / 2.0, m_switchSize,
                                     m_switchSize);
}

void PowerElement::DrawSwitches() const
{
    int i = 0;
    for(auto it = m_parentList.begin(); it != m_parentList.end(); it++) {
        Element* parent = *it;
        if(parent) {
            if(m_online) {
                glColor4dv(m_closedSwitchColour.GetRGBA());
            } else {
                glColor4dv(m_openedSwitchColour.GetRGBA());
            }

            glPushMatrix();
            glTranslated(m_switchRect[i].GetPosition().m_x + m_switchSize / 2.0,
                         m_switchRect[i].GetPosition().m_y + m_switchSize / 2.0, 0.0);
            glRotated(parent->GetAngle(), 0.0, 0.0, 1.0);
            glTranslated(-m_switchRect[i].GetPosition().m_x - m_switchSize / 2.0,
                         -m_switchRect[i].GetPosition().m_y - m_switchSize / 2.0, 0.0);

            DrawRectangle(m_switchRect[i].GetPosition() + wxPoint2DDouble(m_switchSize / 2.0, m_switchSize / 2.0),
                          m_switchSize, m_switchSize);

            glPopMatrix();
        }
        i++;
    }
}

void PowerElement::CalculatePowerFlowPts(std::vector<wxPoint2DDouble> edges)
{
    double arrowRate = 100.0;  // One arrow to each "arrowRate" distance in pixels.

    if(edges.size() < 2) return;

    // Clear all power flow points
    for(int i = 0; i < (int)m_powerFlowArrow.size(); i++) m_powerFlowArrow[i].clear();
    m_powerFlowArrow.clear();

    for(int i = 1; i < (int)edges.size(); i++) {
        wxPoint2DDouble pt1 = edges[i - 1];
        wxPoint2DDouble pt2 = edges[i];

        double angle = std::atan2(pt2.m_y - pt1.m_y, pt2.m_x - pt1.m_x);

        wxPoint2DDouble rotPt2(
            std::cos(-angle) * (pt2.m_x - pt1.m_x) - std::sin(-angle) * (pt2.m_y - pt1.m_y) + pt1.m_x,
            std::sin(-angle) * (pt2.m_x - pt1.m_x) + std::cos(-angle) * (pt2.m_y - pt1.m_y) + pt1.m_y);

        int numArrows = std::abs(pt1.m_x - rotPt2.m_x) / arrowRate;
        if(numArrows == 0) numArrows = 1;

        for(int i = 0; i < numArrows; i++) {
            wxPoint2DDouble arrowCenter(pt1.m_x + ((rotPt2.m_x - pt1.m_x) / double(numArrows + 1)) * double(i + 1),
                                        pt1.m_y + ((rotPt2.m_y - pt1.m_y) / double(numArrows + 1)) * double(i + 1));

            std::vector<wxPoint2DDouble> triPts;
            triPts.push_back(arrowCenter + wxPoint2DDouble(5.0, 0.0));
            triPts.push_back(arrowCenter + wxPoint2DDouble(-5.0, 5.0));
            triPts.push_back(arrowCenter + wxPoint2DDouble(-5.0, -5.0));

            // Rotate back.
            for(int i = 0; i < 3; i++) {
                triPts[i] = wxPoint2DDouble(
                    std::cos(angle) * (triPts[i].m_x - pt1.m_x) - std::sin(angle) * (triPts[i].m_y - pt1.m_y) + pt1.m_x,
                    std::sin(angle) * (triPts[i].m_x - pt1.m_x) + std::cos(angle) * (triPts[i].m_y - pt1.m_y) +
                        pt1.m_y);
            }
            m_powerFlowArrow.push_back(triPts);
        }
    }
}

void PowerElement::DrawPowerFlowPts() const
{
    if(m_online) {
        glColor4dv(m_powerFlowArrowColour.GetRGBA());
        for(int i = 0; i < (int)m_powerFlowArrow.size(); i++) {
            DrawTriangle(m_powerFlowArrow[i]);
        }
    }
}
