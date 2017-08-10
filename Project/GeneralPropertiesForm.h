#ifndef GENERALPROPERTIESFORM_H
#define GENERALPROPERTIESFORM_H

#include "PropertiesForm.h"
class PropertiesData;

class GeneralPropertiesForm : public GeneralPropertiesFormBase
{
public:
    GeneralPropertiesForm(wxWindow* parent, PropertiesData* properties);
    virtual ~GeneralPropertiesForm();
    
protected:
    virtual void OnButtonCancelClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnButtonOKClick(wxCommandEvent& event);
    virtual bool ValidateData();
    
    PropertiesData* m_properties = NULL;
};
#endif // GENERALPROPERTIESFORM_H
