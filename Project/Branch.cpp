#include "Branch.h"

Branch::Branch()
    : Element()
{
}
Branch::~Branch() {}
bool Branch::NodeContains(wxPoint2DDouble position)
{
    wxRect2DDouble nodeRect1(m_pointList[0].m_x - 5.0 - m_borderSize, m_pointList[0].m_y - 5.0 - m_borderSize,
        10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);
    wxRect2DDouble nodeRect2(m_pointList[m_pointList.size() - 1].m_x - 5.0 - m_borderSize,
        m_pointList[m_pointList.size() - 1].m_y - 5.0 - m_borderSize, 10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);

    if(nodeRect1.Contains(position)) {
        m_activeNodeID = 1;
        return true;
    }
    if(nodeRect2.Contains(position)) {
        m_activeNodeID = 2;
        return true;
    }

    m_activeNodeID = 0;
    return false;
}

bool Branch::SetNodeParent(Element* parent)
{
    if(m_activeNodeID == 1 && parent == m_parentList[0]) return false;
    if(m_activeNodeID == 2 && parent == m_parentList[1]) return false;

    if(parent && m_activeNodeID != 0) {
        wxRect2DDouble nodeRect(0, 0, 0, 0);
        if(m_activeNodeID == 1) {
            nodeRect = wxRect2DDouble(m_pointList[0].m_x - 5.0 - m_borderSize, m_pointList[0].m_y - 5.0 - m_borderSize,
                10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);
        }
        if(m_activeNodeID == 2) {
            nodeRect = wxRect2DDouble(m_pointList[m_pointList.size() - 1].m_x - 5.0 - m_borderSize,
                m_pointList[m_pointList.size() - 1].m_y - 5.0 - m_borderSize, 10 + 2.0 * m_borderSize,
                10 + 2.0 * m_borderSize);
        }

        if(parent->Intersects(nodeRect)) {
            if(m_activeNodeID == 1) {
                // Check if the user is trying to connect the same bus.
                if(m_parentList[1] == parent) {
                    m_activeNodeID = 0;
                    return false;
                }

                m_parentList[0] = parent;

                // Centralize the node on bus.
                wxPoint2DDouble parentPt = parent->RotateAtPosition(
                    m_pointList[0], -parent->GetAngle()); // Rotate click to horizontal position.
                parentPt.m_y = parent->GetPosition().m_y; // Centralize on bus.
                parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());
                m_pointList[0] = parentPt;

                UpdateSwitchesPosition();
                return true;
            }
            if(m_activeNodeID == 2) {
                if(m_parentList[0] == parent) {
                    m_activeNodeID = 0;
                    return false;
                }

                m_parentList[1] = parent;

                wxPoint2DDouble parentPt =
                    parent->RotateAtPosition(m_pointList[m_pointList.size() - 1], -parent->GetAngle());
                parentPt.m_y = parent->GetPosition().m_y;
                parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());
                m_pointList[m_pointList.size() - 1] = parentPt;

                UpdateSwitchesPosition();
                return true;
            }
        } else {
            if(m_activeNodeID == 1) m_parentList[0] = NULL;
            if(m_activeNodeID == 2) m_parentList[1] = NULL;
        }
    }
    return false;
}

void Branch::RemoveParent(Element* parent)
{
    for(int i = 0; i < 2; i++) {
        if(parent == m_parentList[i]) {
            m_parentList[i] = NULL;
            m_online = false;
            UpdateSwitchesPosition();
        }
    }
}

void Branch::UpdateNodes()
{
    if(m_parentList[0]) {
        wxRect2DDouble nodeRect(m_pointList[0].m_x - 5.0 - m_borderSize, m_pointList[0].m_y - 5.0 - m_borderSize,
            10 + 2.0 * m_borderSize, 10 + 2.0 * m_borderSize);

        if(!m_parentList[0]->Intersects(nodeRect)) {
            m_parentList[0]->RemoveChild(this);
            m_parentList[0] = NULL;
            m_online = false;
            UpdateSwitchesPosition();
        }
    }
    if(m_parentList[1]) {
        wxRect2DDouble nodeRect = wxRect2DDouble(m_pointList[m_pointList.size() - 1].m_x - 5.0 - m_borderSize,
            m_pointList[m_pointList.size() - 1].m_y - 5.0 - m_borderSize, 10 + 2.0 * m_borderSize,
            10 + 2.0 * m_borderSize);

        if(!m_parentList[1]->Intersects(nodeRect)) {
            m_parentList[1]->RemoveChild(this);
            m_parentList[1] = NULL;
            m_online = false;
            UpdateSwitchesPosition();
        }
    }
}

void Branch::RotateNode(Element* parent, bool clockwise)
{
    double rotAngle = m_rotationAngle;
    if(!clockwise) rotAngle = -m_rotationAngle;

    if(parent == m_parentList[0]) {
        m_pointList[0] = parent->RotateAtPosition(m_pointList[0], rotAngle);
    } else if(parent == m_parentList[1]) {
        m_pointList[m_pointList.size() - 1] = parent->RotateAtPosition(m_pointList[m_pointList.size() - 1], rotAngle);
    }
    UpdateSwitchesPosition();
}

void Branch::UpdateSwitchesPosition()
{
    if(m_parentList[0]) {
        m_pointList[1] = GetSwitchPoint(m_parentList[0], m_pointList[0], m_pointList[2]);
    } else {
        m_pointList[1] = m_pointList[0];
    }
    if(m_parentList[1]) {
        m_pointList[m_pointList.size() - 2] =
            GetSwitchPoint(m_parentList[1], m_pointList[m_pointList.size() - 1], m_pointList[m_pointList.size() - 3]);
    } else {
        m_pointList[m_pointList.size() - 2] = m_pointList[m_pointList.size() - 1];
    }
    UpdateSwitches();
}

void Branch::UpdateSwitches()
{
    wxPoint2DDouble swCenter = wxPoint2DDouble(
        (m_pointList[0].m_x + m_pointList[1].m_x) / 2.0, (m_pointList[0].m_y + m_pointList[1].m_y) / 2.0);
    m_switchRect[0] = wxRect2DDouble(
        swCenter.m_x - m_switchSize / 2.0, swCenter.m_y - m_switchSize / 2.0, m_switchSize, m_switchSize);

    if(m_switchRect.size() > 1) {
        swCenter =
            wxPoint2DDouble((m_pointList[m_pointList.size() - 1].m_x + m_pointList[m_pointList.size() - 2].m_x) / 2.0,
                (m_pointList[m_pointList.size() - 1].m_y + m_pointList[m_pointList.size() - 2].m_y) / 2.0);
        m_switchRect[1] = wxRect2DDouble(
            swCenter.m_x - m_switchSize / 2.0, swCenter.m_y - m_switchSize / 2.0, m_switchSize, m_switchSize);
    }
}
