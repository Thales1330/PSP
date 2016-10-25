#ifndef REACTIVESHUNTELEMENTFORM_H
#define REACTIVESHUNTELEMENTFORM_H
#include "ElementForm.h"
#include <wx/log.h>

class Capacitor;
class Inductor;
class SwitchingForm;

class ReactiveShuntElementForm : public ReactiveShuntElementFormBase
{
   public:
    ReactiveShuntElementForm(wxWindow* parent, Capacitor* capacitor);
    ReactiveShuntElementForm(wxWindow* parent, Inductor* inductor);
    virtual ~ReactiveShuntElementForm();
    virtual bool ValidateData();

   protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnOKButtonClick(wxCommandEvent& event);
    virtual void OnStabilityButtonClick(wxCommandEvent& event);

    wxWindow* m_parent;
    Capacitor* m_capacitor = NULL;
    Inductor* m_inductor = NULL;
};
#endif  // REACTIVESHUNTELEMENTFORM_H
