#ifndef BUSFORMARCFLASHPROTECTION_H
#define BUSFORMARCFLASHPROTECTION_H
#include "ElementFormBase.h"
#include <wx/popupwin.h>

struct ArcFlashProtectionData;
class Bus;
class TCCPopup;

class BusFormArcFlashProtection : public BusFormArcFlashProtectionBase
{
public:
    BusFormArcFlashProtection(wxWindow* parent, Bus* bus, double basePower);
    ~BusFormArcFlashProtection() override;

private:
    void EnableFields();
protected:
    void OnChoiceFuseClass(wxCommandEvent& event) override;
    void OnNominalCurrentUpdated(wxCommandEvent& event) override;
    void OnRadioI1Selected(wxCommandEvent& event) override;
    void OnChoiceCBCurrent(wxCommandEvent& event) override;
    void OnChoiceCBTrip(wxCommandEvent& event) override;
    void OnChoiceCBType(wxCommandEvent& event) override;
    void OnAddButtonClick(wxCommandEvent& event) override;
    void OnDeviceChanged(wxDataViewEvent& event) override;
    void OnImportButtonClick(wxCommandEvent& event) override;
    void OnMethodSelected(wxCommandEvent& event) override;
    void OnOKButtonClick(wxCommandEvent& event) override;
    void OnRemoveButtonClick(wxCommandEvent& event) override;
    void OnTypeSelected(wxCommandEvent& event) override;
    void OnnCancelButtonClick(wxCommandEvent& event) override;
    void OnGridKeyDown(wxKeyEvent& event);
    void OnGridCornerPaint(wxPaintEvent& event);
    bool ParseDouble(const wxString& text, double& value);
    void InitializeCBChoices();
    void UpdateCBCurrentChoices();
    void UpdateCBTripChoices();
    void UpdateCBChoices();
    void UpdateRadioI1Selection();
    void UpdateFuseCurrentChoices();
    void LoadProtectionData(const ArcFlashProtectionData& data);
    bool ValidateAndGetProtectionData(ArcFlashProtectionData& data);

    Bus* m_bus = nullptr;
	double m_basePower = 100e6; // Default base power is 100 MVA
    TCCPopup* m_tccPopup = nullptr;
    bool m_gridCornerHover = false;
    wxDataViewItem m_previousDeviceSelection;
    bool m_restoringDeviceSelection = false;
};

class TCCPopup : public wxPopupTransientWindow
{
public:
    TCCPopup(wxWindow* parent, wxGrid* grid);
    ~TCCPopup();

    void UpdateGraph();

private:
    void OnPaint(wxPaintEvent& event);
    bool GetCellDouble(int row, int col, double& value) const;

    wxGrid* m_grid = nullptr;
};

#endif // BUSFORMARCFLASHPROTECTION_H
