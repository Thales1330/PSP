#include "Line.h"

Line::Line() {}
Line::~Line() {}
bool Line::Contains(wxPoint2DDouble position) const {}
void Line::Draw(wxPoint2DDouble translation, double scale) const {}
wxCursor Line::GetBestPickboxCursor() const {}
void Line::Insert(Element* parent, wxPoint2DDouble position) {}
bool Line::Intersects(wxRect2DDouble rect) const {}
void Line::MovePickbox(wxPoint2DDouble position) {}
bool Line::PickboxContains(wxPoint2DDouble position) {}
void Line::Rotate() {}
