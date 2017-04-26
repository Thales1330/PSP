#ifndef CHARTVIEW_H
#define CHARTVIEW_H
#include "ChartViewBase.h"
#include "wxMathPlot/mathplot.h"

#include <wx/msgdlg.h>

class ChartView : public ChartViewBase
{
   public:
    ChartView(wxWindow* parent);
    virtual ~ChartView();
    
    void Fit();
    void UpdatePlot(bool fit = true);

   protected:
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
    mpWindow* m_mpWindow = NULL;
    mpScaleX* m_xaxis = NULL;
    mpScaleY* m_yaxis = NULL;
    mpText* m_chartTitle = NULL;
    mpInfoCoords* m_coords = NULL;
    mpInfoLegend* m_leg = NULL;

    bool m_hideGrid = false;
    bool m_showLeg = false;
    bool m_showCoords = false;
    bool m_darkTheme = false;

    wxPGProperty* m_pgPropColor = NULL;
};
#endif  // CHARTVIEW_H
