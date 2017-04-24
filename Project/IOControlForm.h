#ifndef IOCONTROLFORM_H
#define IOCONTROLFORM_H

#include "ElementForm.h"
#include "IOControl.h"

class IOControlForm : public IOControlFormBase
{
   public:
    IOControlForm(wxWindow* parent, IOControl* ioControl);
    virtual ~IOControlForm();
    
    virtual bool ValidateData();

   protected:
    virtual void OnInputChecked(wxCommandEvent& event);
    virtual void OnOutputChecked(wxCommandEvent& event);
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); };
    virtual void OnOKButtonClick(wxCommandEvent& event);

    wxWindow* m_parent;
    IOControl* m_ioControl;
    
    std::vector<IOControl::IOFlags> m_inputFlags;
    std::vector<IOControl::IOFlags> m_outputFlags; 
};
#endif  // IOCONTROLFORM_H
