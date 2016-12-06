#ifndef INDMOTORFORM_H
#define INDMOTORFORM_H
#include "ElementForm.h"

class IndMotor;

class IndMotorForm : public IndMotorFormBase
{
   public:
    IndMotorForm(wxWindow* parent, IndMotor* indMotor);
    virtual ~IndMotorForm();
    virtual bool ValidateData();

   protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); };
    virtual void OnOKButtonClick(wxCommandEvent& event);
    virtual void OnStabilityButtonClick(wxCommandEvent& event);

    wxWindow* m_parent;
    IndMotor* m_indMotor;
};
#endif  // INDMOTORFORM_H
