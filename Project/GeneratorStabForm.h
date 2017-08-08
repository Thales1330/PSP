#ifndef GENERATORSTABFORM_H
#define GENERATORSTABFORM_H

#include "ElementForm.h"

class SwitchingForm;
class SyncGenerator;
class ControlEditor;
class ControlElementContainer;

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

    SyncGenerator* m_syncGenerator = NULL;
    wxWindow* m_parent = NULL;
};
#endif  // GENERATORSTABFORM_H
