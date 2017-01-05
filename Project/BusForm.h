#ifndef BUSFORM_H
#define BUSFORM_H

#include "ElementForm.h"
class Bus;

class BusForm : public BusFormBase
{
public:
    BusForm(wxWindow* parent, Bus* bus);
    virtual ~BusForm();
protected:
    virtual void OnFaultTypeChoice(wxCommandEvent& event);
    virtual void OnControlledVoltageClick(wxCommandEvent& event);
    virtual void OnInsertFaultClick(wxCommandEvent& event);
    virtual void OnInsertStabFaultClick(wxCommandEvent& event);
    virtual void OnNominalVoltageChoice(wxCommandEvent& event);
    virtual void OnButtonCancelClick(wxCommandEvent& event);
    virtual void OnButtonOKClick(wxCommandEvent& event);
	virtual void EnableCtrlVoltageFields(bool enable = true);
	virtual void EnableFaultFields(bool enable = true);
	virtual void EnableStabFaultFields(bool enable = true);
	virtual void UpdateChoiceBoxes();
	
	Bus* m_bus = NULL;
	wxWindow* m_parent = NULL;
};
#endif // BUSFORM_H
