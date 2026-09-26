#ifndef BUSFORMARCFLASHPROTECTION_H
#define BUSFORMARCFLASHPROTECTION_H
#include "ElementFormBase.h"

class Bus;

class BusFormArcFlashProtection : public BusFormArcFlashProtectionBase
{
public:
    BusFormArcFlashProtection(wxWindow* parent, Bus* bus, double basePower);
    ~BusFormArcFlashProtection() override;

private:
    void EnableFields();
protected:
    void OnAddButtonClick(wxCommandEvent& event) override;
    void OnDeviceChanged(wxDataViewEvent& event) override;
    void OnImportButtonClick(wxCommandEvent& event) override;
    void OnMethodSelected(wxCommandEvent& event) override;
    void OnOKButtonClick(wxCommandEvent& event) override;
    void OnRemoveButtonClick(wxCommandEvent& event) override;
    void OnTypeSelected(wxCommandEvent& event) override;
    void OnnCancelButtonClick(wxCommandEvent& event) override;

    Bus* m_bus;
	double m_basePower = 100e6; // Default base power is 100 MVA
};
#endif // BUSFORMARCFLASHPROTECTION_H
