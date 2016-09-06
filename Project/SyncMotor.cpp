#include "SyncMotor.h"

SyncMotor::SyncMotor() : Machines()
{
}

SyncMotor::~SyncMotor()
{
}

void SyncMotor::DrawSymbol() const
{
	DrawArc(m_position, 12, 30, 330, 10, GL_LINE_STRIP);
}

bool SyncMotor::GetContextMenu(wxMenu& menu)
{
	menu.Append(ID_EDIT_SYNCMOTOR, _("Edit Synchronous Condenser"));
	
	wxMenuItem* clockItem = new wxMenuItem(&menu, ID_ROTATE_CLOCK, _("Rotate clockwise"));
	clockItem->SetBitmap(wxImage("data\\images\\menu\\rotateClock16.png"));
	menu.Append(clockItem);
	
	wxMenuItem* counterClockItem = new wxMenuItem(&menu, ID_ROTATE_COUNTERCLOCK, _("Rotate counter-clockwise"));
	counterClockItem->SetBitmap(wxImage("data\\images\\menu\\rotateCounterClock16.png"));
	menu.Append(counterClockItem);
	
    wxMenuItem* deleteItem = new wxMenuItem(&menu, ID_DELETE, _("Delete"));
	deleteItem->SetBitmap(wxImage("data\\images\\menu\\delete16.png"));
	menu.Append(deleteItem);
    return true;
}
