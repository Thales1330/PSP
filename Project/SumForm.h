#ifndef SUMFORM_H
#define SUMFORM_H

#include "ElementForm.h"

class Sum;

class SumForm : public SumFormBase
{
public:
    SumForm(wxWindow* parent, Sum* sum);
    virtual ~SumForm();
    
    bool ValidateData();
    
protected:
    virtual void OnCancelClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnOKClick(wxCommandEvent& event);
    wxWindow* m_parent = NULL;
    Sum* m_sum = NULL;
};
#endif // SUMFORM_H
