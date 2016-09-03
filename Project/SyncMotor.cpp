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
    menu.Append(ID_ROTATE, _("Rotate"));
    menu.Append(ID_DELETE, _("Delete"));
    return true;
}
