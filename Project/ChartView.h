/*
 *  Copyright (C) 2017  Thales Lima Oliveira <thales@ufu.br>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CHARTVIEW_H
#define CHARTVIEW_H
#include "ChartViewBase.h"
#include "wxMathPlot/mathplot.h"

#include <wx/msgdlg.h>
#include <wx/clipbrd.h>
#include <wx/bitmap.h>
#include <wx/dcscreen.h>
#include <wx/filedlg.h>
#include <wx/textfile.h>

class PlotData;
class ElementPlotData;

/**
 * @class ChartView
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief This class is responsible to manage the charts generated in the transient electromechanical studies.
 * @file ChartView.h
 */
class ChartView : public ChartViewBase
{
   public:
    ChartView(wxWindow* parent, std::vector<ElementPlotData> epdList, std::vector<double> time);
    virtual ~ChartView();

    void Fit();
    void UpdatePlot(bool fit = true);

   protected:
    virtual void OnMenuExpCSVClick(wxCommandEvent& event);
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
    virtual wxString GetActiveCurvesCSV();
    virtual wxTreeItemId GetActivePlotData(wxTreeItemId root, std::vector<PlotData*>& plotDataList);

    wxPGProperty* m_pgPropColor = nullptr;

    wxTreeItemId m_treeTimeID;

    std::vector<ElementPlotData> m_epdList;
    std::vector<double> m_time;
    std::vector<double> m_xAxisValues;

    mpWindow* m_mpWindow = nullptr;
    mpScaleX* m_xaxis = nullptr;
    mpScaleY* m_yaxis = nullptr;
    mpText* m_chartTitle = nullptr;
    mpInfoCoords* m_coords = nullptr;
    mpInfoLegend* m_leg = nullptr;

    bool m_hideGrid = true;
    bool m_showLeg = true;
    bool m_showCoords = false;
    bool m_darkTheme = false;

    std::vector<wxColour> m_colourList;
    std::vector<wxColour>::iterator m_itColourList;
};
#endif  // CHARTVIEW_H
