#ifndef GENERALPROPERTIESFORM_H
#define GENERALPROPERTIESFORM_H
#include "PropertiesForm.h"

class GeneralPropertiesForm : public GeneralPropertiesFormBase
{
public:
    GeneralPropertiesForm(wxWindow* parent);
    virtual ~GeneralPropertiesForm();
protected:
    virtual void OnButtonCancelClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnButtonOKClick(wxCommandEvent& event);
    virtual bool ValidateData();
};
#endif // GENERALPROPERTIESFORM_H
