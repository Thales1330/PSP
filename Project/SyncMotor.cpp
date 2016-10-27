#include "SyncMachineForm.h"
#include "SyncMotor.h"

SyncMotor::SyncMotor() : Machines() {}
SyncMotor::SyncMotor(wxString name) : Machines() { m_electricalData.name = name; }
SyncMotor::~SyncMotor() {}
void SyncMotor::DrawSymbol() const { DrawArc(m_position, 12, 30, 330, 10, GL_LINE_STRIP); }
bool SyncMotor::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_SYNCMOTOR, _("Edit Synchronous Condenser"));
    GeneralMenuItens(menu);

    return true;
}

bool SyncMotor::ShowForm(wxWindow* parent, Element* element)
{
    SyncMachineForm* syncMotorForm = new SyncMachineForm(parent, this);
    syncMotorForm->SetTitle(_("Synchronous Condenser"));
    if(syncMotorForm->ShowModal() == wxID_OK) {
        syncMotorForm->Destroy();
        return true;
    }

    syncMotorForm->Destroy();
    return false;
}
