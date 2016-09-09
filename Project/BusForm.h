#ifndef BUSFORM_H
#define BUSFORM_H
#include "ElementForm.h"

class BusForm : public BusFormBase
{
public:
    BusForm(wxWindow* parent);
    virtual ~BusForm();
protected:
    virtual void OnButtonCancelClick(wxCommandEvent& event);
    virtual void OnButtonOKClick(wxCommandEvent& event);
};
#endif // BUSFORM_H
