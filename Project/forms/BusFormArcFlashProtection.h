#ifndef BUSFORMARCFLASHPROTECTION_H
#define BUSFORMARCFLASHPROTECTION_H
#include "ElementFormBase.h"
#include <wx/popupwin.h>

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

    Bus* m_bus = nullptr;
	double m_basePower = 100e6; // Default base power is 100 MVA
    TCCPopup* m_tccPopup = nullptr;
    bool m_gridCornerHover = false;
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
