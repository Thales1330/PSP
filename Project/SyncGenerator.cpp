#include "SyncGenerator.h"

SyncGenerator::SyncGenerator() : Machines()
{
    int numPtsSine = 10;
    double mx = 15.0;
    double my = 10.0;
    double pi = 3.14159265359;

    for(int i = 0; i <= numPtsSine; i++) {
	    double x = (2.0 * pi / double(numPtsSine)) * double(i) - pi;
	    double y = std::sin(x);
	    m_sinePts.push_back(wxPoint2DDouble((x / pi) * mx, y * my));
	}
}
SyncGenerator::~SyncGenerator() {}

void SyncGenerator::DrawSymbol() const
{
    // Draw sine.
	std::vector<wxPoint2DDouble> sinePts;
	for(int i = 0; i < (int)m_sinePts.size(); i++) {
		sinePts.push_back(m_sinePts[i] + m_position);
	}
	DrawLine(sinePts);
}
bool SyncGenerator::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_SYNCGENERATOR, _("Edit Generator"));
    menu.Append(ID_ROTATE_CLOCK, _("Rotate clockwise"));
	menu.Append(ID_ROTATE_COUNTERCLOCK, _("Rotate counter-clockwise"));
    menu.Append(ID_DELETE, _("Delete"));
    return true;
}
