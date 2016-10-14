#include "TransformerForm.h"
#include "SwitchingForm.h"
#include "Transformer.h"

TransformerForm::TransformerForm(wxWindow* parent, Transformer* transformer)
    : TransformerFormBase(parent)
{
    m_parent = parent;
    m_transformer = transformer;
}

TransformerForm::~TransformerForm()
{
}

void TransformerForm::OnCancelButtonClick(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}
void TransformerForm::OnOKButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}
void TransformerForm::OnStabilityButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) {
        SwitchingForm swForm(m_parent, m_transformer);
        swForm.SetTitle(_("Transfomer: Switching"));
        swForm.ShowModal();
        EndModal(wxID_OK);
    }
}

bool TransformerForm::ValidateData()
{
    return true;
}
