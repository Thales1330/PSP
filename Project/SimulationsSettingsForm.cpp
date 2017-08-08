#include "SimulationsSettingsForm.h"

SimulationsSettingsForm::SimulationsSettingsForm(wxWindow* parent)
    : SimulationsSettingsFormBase(parent)
{
}

SimulationsSettingsForm::~SimulationsSettingsForm()
{
}

void SimulationsSettingsForm::OnButtonOKClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

bool SimulationsSettingsForm::ValidateData()
{
    return true;
}
