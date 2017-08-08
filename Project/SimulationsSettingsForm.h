#ifndef SIMULATIONSSETTINGSFORM_H
#define SIMULATIONSSETTINGSFORM_H
#include "PropertiesForm.h"

class SimulationsSettingsForm : public SimulationsSettingsFormBase
{
public:
    SimulationsSettingsForm(wxWindow* parent);
    virtual ~SimulationsSettingsForm();
protected:
    virtual void OnButtonCancelClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnButtonOKClick(wxCommandEvent& event);
    virtual bool ValidateData();
};
#endif // SIMULATIONSSETTINGSFORM_H
