#ifndef TEXTFORM_H
#define TEXTFORM_H
#include "ElementForm.h"

#include "Text.h"
#include "ElectricCalculation.h"

class TextForm : public TextFormBase
{
   public:
    TextForm(wxWindow* parent, Text* text, std::vector<Element*> elementList, double systemPowerBase = 100e6);
    virtual ~TextForm();
    virtual void ElementTypeChoice();
    virtual void ElementNumberChoice();
    virtual void DataTypeChoice();
    virtual void UnitChoice();

   protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnOKButtonClick(wxCommandEvent& event) { EndModal(wxID_OK); }
    virtual void OnElementChoiceSelected(wxCommandEvent& event);
    virtual void OnFromBusChoiceSelected(wxCommandEvent& event);
    virtual void OnNameChoiceSelected(wxCommandEvent& event);
    virtual void OnTextEnter(wxCommandEvent& event);
    virtual void OnToBusChoiceSelected(wxCommandEvent& event);
    virtual void OnTypeChoiceSelected(wxCommandEvent& event);

    Text* m_text;
    wxWindow* m_parent;
    ElectricCalculation m_allElements;
    double m_systemPowerBase;
};
#endif  // TEXTFORM_H
