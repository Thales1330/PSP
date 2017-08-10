#ifndef SIMULATIONSSETTINGSFORM_H
#define SIMULATIONSSETTINGSFORM_H

#include "PropertiesForm.h"

class PropertiesData;

class SimulationsSettingsForm : public SimulationsSettingsFormBase
{
public:
    SimulationsSettingsForm(wxWindow* parent, PropertiesData* properties);
    virtual ~SimulationsSettingsForm();
protected:
    virtual void OnPFMethodChoiceSelected(wxCommandEvent& event);
    virtual void OnButtonCancelClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnButtonOKClick(wxCommandEvent& event);
    virtual bool ValidateData();
    
    PropertiesData* m_properties;
};
#endif // SIMULATIONSSETTINGSFORM_H
