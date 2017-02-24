#include "ConnectionLine.h"

ConnectionLine::ConnectionLine()
    : ControlElement()
{
}

ConnectionLine::~ConnectionLine() {}

void ConnectionLine::Draw(wxPoint2DDouble translation, double scale) const
{
    
}

bool ConnectionLine::Contains(wxPoint2DDouble position) const
{
    if(PointToLineDistance(position) < 5.0) {
        return true;
    }
    return false;
}

bool ConnectionLine::Intersects(wxRect2DDouble rect) const
{
    for(auto it = m_pointList.begin(); it != m_pointList.end(); ++it) {
        if(rect.Contains(*it)) return true;
    }
    return false;
}
