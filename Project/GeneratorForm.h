#ifndef GENERATORFORM_H
#define GENERATORFORM_H

#include "ElementForm.h"

class GeneratorStabForm;
class SyncGenerator;

class GeneratorForm : public GeneratorFormBase
{
   public:
    GeneratorForm(wxWindow* parent, SyncGenerator* syncGenerator);
    virtual ~GeneratorForm();
	
protected:
    virtual void OnStabilityButtonClick(wxCommandEvent& event);
    virtual void OnCancelButtonClick(wxCommandEvent& event);
    virtual void OnCheckMaxReactive(wxCommandEvent& event);
    virtual void OnCheckMinReactive(wxCommandEvent& event);
    virtual void OnOKButtonClick(wxCommandEvent& event);
	
	virtual bool ValidateData();
	virtual void ReplaceStaticTextLabelChar(wxStaticText* staticText, wchar_t newChar);

	SyncGenerator* m_syncGenerator;
	wxWindow* m_parent;
	
};
#endif  // GENERATORFORM_H
