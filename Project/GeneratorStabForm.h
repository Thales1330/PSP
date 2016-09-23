#ifndef GENERATORSTABFORM_H
#define GENERATORSTABFORM_H

#include "ElementForm.h"
class SyncGenerator;

class GeneratorStabForm : public GeneratorStabFormBase
{
public:
    GeneratorStabForm(wxWindow* parent, SyncGenerator* syncGenerator);
    virtual ~GeneratorStabForm();
	
protected:
    virtual void UseAVRClick(wxCommandEvent& event);
    virtual void UseSGClick(wxCommandEvent& event);
    virtual void OnCancelButtonClick(wxCommandEvent& event);
    virtual void OnEditAVRButtonClick(wxCommandEvent& event);
    virtual void OnOKButtonClick(wxCommandEvent& event);
    virtual void OnSpeedGovernorButtonClick(wxCommandEvent& event);
    virtual void OnSwitchingButtonClick(wxCommandEvent& event);
	
	virtual bool ValidateData();
	
	SyncGenerator* m_syncGenerator;
	wxWindow* m_parent;
};
#endif // GENERATORSTABFORM_H
