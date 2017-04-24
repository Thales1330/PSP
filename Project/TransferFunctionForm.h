#ifndef TRANSFERFUNCTIONFORM_H
#define TRANSFERFUNCTIONFORM_H

#include "ElementForm.h"

class TransferFunction;

class TransferFunctionForm : public TransferFunctionFormBase
{
public:
    TransferFunctionForm(wxWindow* parent, TransferFunction* transferFunction);
    virtual ~TransferFunctionForm();
    bool ValidateData();

protected:
    virtual void OnCancelClick(wxCommandEvent& event);
    virtual void OnOKClick(wxCommandEvent& event);
    void LoadTFData();
    
    wxWindow* m_parent = NULL;
    TransferFunction* m_tf = NULL;
};
#endif // TRANSFERFUNCTIONFORM_H
