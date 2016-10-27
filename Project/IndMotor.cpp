#include "IndMotorForm.h"
#include "IndMotor.h"

IndMotor::IndMotor() : Machines() {}
IndMotor::IndMotor(wxString name) : Machines() { m_electricalData.name = name; }
IndMotor::~IndMotor() {}
void IndMotor::DrawSymbol() const
{
    std::vector<wxPoint2DDouble> mPts;
    mPts.push_back(wxPoint2DDouble(-10, 13) + m_position);
    mPts.push_back(wxPoint2DDouble(-10, -13) + m_position);
    mPts.push_back(wxPoint2DDouble(0, 2) + m_position);
    mPts.push_back(wxPoint2DDouble(10, -13) + m_position);
    mPts.push_back(wxPoint2DDouble(10, 13) + m_position);
    DrawLine(mPts);
}

bool IndMotor::GetContextMenu(wxMenu& menu)
{
    menu.Append(ID_EDIT_INDMOTOR, _("Edit induction motor"));
    GeneralMenuItens(menu);
    return true;
}

bool IndMotor::ShowForm(wxWindow* parent, Element* element)
{
    IndMotorForm* indMotorForm = new IndMotorForm(parent, this);
    if(indMotorForm->ShowModal() == wxID_OK) {
        indMotorForm->Destroy();
        return true;
    }
    indMotorForm->Destroy();
    return false;
}
