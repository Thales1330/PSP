#ifndef CHARTVIEW_H
#define CHARTVIEW_H
#include "ChartViewBase.h"
#include "wxMathPlot/mathplot.h"

#include <wx/msgdlg.h>
#include <wx/clipbrd.h>
#include <wx/bitmap.h>
#include <wx/dcscreen.h>
#include <wx/filedlg.h>

class ElementPlotData;

class ChartView : public ChartViewBase
{
   public:
    ChartView(wxWindow* parent, std::vector<ElementPlotData> epdList, std::vector<double> time);
    virtual ~ChartView();

    void Fit();
    void UpdatePlot(bool fit = true);

   protected:
    virtual void OnTreeItemActivated(wxTreeEvent& event);
    virtual void OnTreeItemSelectionChanged(wxTreeEvent& event);
    virtual void OnMenuDarkThemeClick(wxCommandEvent& event);
    virtual void OnMenuExitClick(wxCommandEvent& event) { Close(); }
    virtual void OnMenuFitClick(wxCommandEvent& event) { Fit(); }
    virtual void OnMenuSaveImageClick(wxCommandEvent& event);
    virtual void OnMenuSendClipClick(wxCommandEvent& event);
    virtual void OnMenuShowCoordinatesClick(wxCommandEvent& event);
    virtual void OnMenuShowGridClick(wxCommandEvent& event);
    virtual void OnMenuShowLabelClick(wxCommandEvent& event);
    virtual void OnPropertyGridChange(wxPropertyGridEvent& event);
    virtual void SetMPWindow();
    virtual void SetTreectrl();
    virtual void BuildColourList();
    virtual wxColour GetNextColour();
    virtual wxTreeItemId AllToYAxis(wxTreeItemId root);
    virtual wxTreeItemId UpdateAllPlots(wxTreeItemId root);

    wxPGProperty* m_pgPropColor = NULL;

    wxTreeItemId m_treeTimeID;

    std::vector<ElementPlotData> m_epdList;
    std::vector<double> m_time;
    std::vector<double> m_xAxisValues;

    mpWindow* m_mpWindow = NULL;
    mpScaleX* m_xaxis = NULL;
    mpScaleY* m_yaxis = NULL;
    mpText* m_chartTitle = NULL;
    mpInfoCoords* m_coords = NULL;
    mpInfoLegend* m_leg = NULL;

    bool m_hideGrid = true;
    bool m_showLeg = true;
    bool m_showCoords = false;
    bool m_darkTheme = false;

    std::vector<wxColour> m_colourList;
    std::vector<wxColour>::iterator m_itColourList;
};
#endif  // CHARTVIEW_H
