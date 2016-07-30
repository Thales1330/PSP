#include "Bus.h"

Bus::Bus(wxPoint2DDouble position) : Element() 
{
	m_width = 100.0;
	m_height = 5.0;
	SetPosition(position);	
}
Bus::~Bus() {}

void Bus::Draw(wxPoint2DDouble translation, double scale) const
{
	glColor4d(0.0, 0.0, 1.0, 1.0);
	DrawRectangle(m_position, m_width, m_height);
}
bool Bus::Contains(wxPoint2DDouble position) const { return false; }
int Bus::PickboxContains(wxPoint2DDouble position) const { return 0; }
wxCursor Bus::GetBestPickboxCursor() const { return wxCURSOR_ARROW; }
void Bus::MovePickbox(wxPoint2DDouble position, int pickboxID) {}
