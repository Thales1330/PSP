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

#include <map>

#include <wx/msgdlg.h>
#include <wx/clipbrd.h>
#include <wx/bitmap.h>
#include <wx/dcscreen.h>
#include <wx/filedlg.h>
#include <wx/textfile.h>

#include "../extLibs/wxMathPlot/mathplot.h"
#include "../extLibs/wxChartDir/wxchartviewer.h"

#include "../utils/PropertiesData.h"

#define ID_CHARTVIEWER 11000

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
	ChartView(wxWindow* parent, std::vector<ElementPlotData> epdList, std::vector<double> time, PlotLib plotLib = PlotLib::wxMATH_PLOT);
	virtual ~ChartView();

	void Fit();
	void UpdatePlot(bool fit = true);

protected:
	virtual void OnResize(wxSizeEvent& event);
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
	virtual void OnRightClickDown(wxMouseEvent& event);
	virtual void SetMPWindow();
	virtual void SetTreectrl();
	virtual void BuildColourList();
	virtual wxColour GetNextColour();
	virtual wxTreeItemId AllToYAxis(wxTreeItemId root);
	virtual wxTreeItemId UpdateAllPlots(wxTreeItemId root);
	virtual wxTreeItemId UpdateAllPlotsCharDir(wxTreeItemId root, XYChart* chartDir);
	virtual wxString GetActiveCurvesCSV();
	virtual wxTreeItemId GetActivePlotData(wxTreeItemId root, std::vector<PlotData*>& plotDataList);
	virtual void DrawChartDir();
	virtual void OnViewPortChanged(wxCommandEvent& event);
	virtual void OnMouseMovePlotArea(wxCommandEvent& event);
	virtual void DrawTrackLine(XYChart* chartDir);
	virtual char DotOrComma() { return wxString::Format("%.1f", 0.1)[1]; };
	virtual wxString GetUnitFromMagText(wxString magText);

	wxPGProperty* m_pgPropColor = nullptr;

	wxTreeItemId m_treeTimeID;

	std::vector<ElementPlotData> m_epdList;
	std::vector<double> m_time;
	std::vector<double> m_xAxisValues;

	PlotLib m_plotLib = PlotLib::wxMATH_PLOT;

	mpWindow* m_mpWindow = nullptr;
	mpScaleX* m_xaxis = nullptr;
	mpScaleY* m_yaxis = nullptr;
	mpText* m_chartTitle = nullptr;
	mpInfoCoords* m_coords = nullptr;
	mpInfoLegend* m_leg = nullptr;

	wxChartViewer* m_chartViewerDir = nullptr;
	wxBoxSizer* m_sizerChart = nullptr;
	wxSize m_chartViewerSize = wxSize(200, 200);
	int m_legendHeight = 0;
	int m_trackLinePos = 0;
	double m_xMin = 0.0;
	double m_xMax = 0.0;
	double m_yMin = 0.0;
	double m_yMax = 0.0;
	bool m_firstPlot = true;
	bool m_forceAxisLimits = false;
	wxString m_sugestXLabel = _("Time (s)");
	wxString m_sugestYLabel = "";
	std::map<int, int> m_rawDataColour;

	bool m_hideGrid = false;
	bool m_showLeg = true;
	bool m_showCoords = false;
	bool m_darkTheme = false;

	std::vector<wxColour> m_colourList;
	std::vector<wxColour>::iterator m_itColourList;
};
#endif  // CHARTVIEW_H
