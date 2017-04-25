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

   protected:
    virtual void OnPropertyGridChange(wxPropertyGridEvent& event);
    virtual void SetMPWindow();
    mpWindow* m_mpWindow = NULL;
    mpScaleX* m_xaxis = NULL;
    mpScaleY* m_yaxis = NULL;
    mpText* m_chartTitle = NULL;
    mpInfoCoords* m_coords = NULL;
    mpInfoLegend* m_leg = NULL;

    wxPGProperty* m_pgPropColor = NULL;
};
#endif  // CHARTVIEW_H
