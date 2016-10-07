#ifndef SWITCHINGFORM_H
#define SWITCHINGFORM_H
#include "ElementForm.h"

class SwitchingForm : public SwitchingFormBase
{
public:
    SwitchingForm(wxWindow* parent);
    virtual ~SwitchingForm();
protected:
    virtual void OnChangeProperties(wxPropertyGridEvent& event);
    virtual void OnSelectItem(wxListEvent& event);
    virtual void OnCancelButtonClick(wxCommandEvent& event);
    virtual void OnInsertButtonClick(wxCommandEvent& event);
    virtual void OnOKButtonClick(wxCommandEvent& event);
    virtual void OnRemoveButtonClick(wxCommandEvent& event);
    
    int m_maxID = 0;
};
#endif // SWITCHINGFORM_H
