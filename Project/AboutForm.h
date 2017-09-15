#ifndef ABOUTFORM_H
#define ABOUTFORM_H
#include "PropertiesForm.h"

#include <wx/textfile.h>

class AboutForm : public AboutFormBase
{
   public:
    AboutForm(wxWindow* parent);
    virtual ~AboutForm();
    virtual void Init();

   protected:
    virtual void OnOKButtonClick(wxCommandEvent& event) { EndModal(wxID_OK); };
};
#endif  // ABOUTFORM_H
