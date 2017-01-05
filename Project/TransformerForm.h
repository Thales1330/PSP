#ifndef TRANSFORMERFORM_H
#define TRANSFORMERFORM_H

#include "ElementForm.h"

class SwitchingForm;
class Transformer;

class TransformerForm : public TransformerFormBase
{
   public:
    TransformerForm(wxWindow* parent, Transformer* transformer);
    virtual ~TransformerForm();
    bool ValidateData();

   protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event);
    virtual void OnOKButtonClick(wxCommandEvent& event);
    virtual void OnStabilityButtonClick(wxCommandEvent& event);

    wxWindow* m_parent = NULL;
    Transformer* m_transformer = NULL;
};
#endif  // TRANSFORMERFORM_H
