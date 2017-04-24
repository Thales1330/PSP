#ifndef CONSTANTFORM_H
#define CONSTANTFORM_H
#include "ElementForm.h"

class Constant;

class ConstantForm : public ConstantFormBase
{
   public:
    ConstantForm(wxWindow* parent, Constant* constant);
    virtual ~ConstantForm();
    virtual bool ValidateData();

   protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnOKButtonClick(wxCommandEvent& event);
    
    wxWindow* m_parent = NULL;
    Constant* m_constant = NULL;
};
#endif  // CONSTANTFORM_H
