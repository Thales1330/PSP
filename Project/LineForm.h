#ifndef LINEFORM_H
#define LINEFORM_H
#include "ElementForm.h"

class Line;
class SwitchingForm;

class LineForm : public LineFormBase
{
public:
    LineForm(wxWindow* parent, Line* line);
    virtual ~LineForm();
protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event);
    virtual void OnOKButtonClick(wxCommandEvent& event);
    virtual void OnStabilityButtonClick(wxCommandEvent& event);
	virtual void ReplaceStaticTextLabelChar(wxStaticText* staticText, wchar_t newChar);
	virtual bool ValidateData();
	
	wxWindow* m_parent;
	Line* m_line;
	
};
#endif // LINEFORM_H
