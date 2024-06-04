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

#ifndef WORKSPACE_H
#define WORKSPACE_H


#ifdef _MSC_VER
 //#include <windows.h>
#endif // _MSC_VER

#include <wx/dcclient.h>
#include <wx/msgdlg.h>
#include <wx/statusbr.h>
#include <wx/clipbrd.h>
#include <wx/tipwin.h>
#include <wx/stopwatch.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include <map>

#include "WorkspaceBase.h"
#include "../elements/powerElement/Bus.h"

#include "ControlEditor.h"

#ifdef _DEBUG
#include "utils/DebugMainFrame.hpp"
#endif

class Camera;
class Element;
// class Bus;
class Line;
class Transformer;
class SyncGenerator;
class IndMotor;
class SyncMotor;
class Load;
class Inductor;
class Capacitor;
class HarmCurrent;
class ElementDataObject;

class Text;

class PowerFlow;
class Fault;
class Electromechanical;
class PowerQuality;

class ElementPlotData;
class ChartView;
class FrequencyResponseForm;

class PropertiesData;

class HMPlane;
class Shader;
class Renderer;

enum ElementID {
	ID_BUS = 0,
	ID_LINE,
	ID_TRANSFORMER,
	ID_SYNCGENERATOR,
	ID_INDMOTOR,
	ID_SYNCMOTOR,
	ID_LOAD,
	ID_CAPACITOR,
	ID_INDUCTOR,
	ID_HARMCURRENT,
	ID_TEXT,

	NUM_ELEMENTS
};

/**
 * @class Workspace
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 19/09/2017
 * @brief This class manages the graphical and power elements.
 * It is responsible for handling the user's interaction with the elements.
 * @file Workspace.h
 */
class Workspace : public WorkspaceBase
{
public:
	enum class WorkspaceMode : int {
		MODE_EDIT = 0,
		MODE_MOVE_ELEMENT,
		MODE_MOVE_PICKBOX,
		MODE_MOVE_NODE,
		MODE_DRAG,
		MODE_DRAG_INSERT,
		MODE_DRAG_INSERT_TEXT,
		MODE_INSERT,
		MODE_INSERT_TEXT,
		MODE_SELECTION_RECT,
		MODE_PASTE,
		MODE_DRAG_PASTE
	};

	Workspace();
	Workspace(wxWindow* parent, wxString name = wxEmptyString, wxStatusBar* statusBar = nullptr);
	virtual ~Workspace();

	wxString GetName() const { return m_name; }
	std::vector<Element*> GetElementList() const;
	std::vector<Text*> GetTextList() const { return m_textList; }
	std::vector<Element*> GetAllElements() const;
	WorkspaceMode GetWorkspaceMode() const { return m_mode; }
	Camera* GetCamera() const { return m_camera; }
	void CopySelection();
	bool Paste();
	void SaveCurrentState();
	void SetNextState();
	void SetPreviousState();

	wxFileName GetSavedPath() const { return m_savedPath; }
	void SetName(wxString name);
	void SetElementList(std::vector<Element*> elementList);
	void SetTextList(std::vector<Text*> textList);
	void SetStatusBarText(wxString text) { m_statusBar->SetStatusText(text); }
	void SetWorkspaceMode(WorkspaceMode mode) { m_mode = mode; }
	void SetSavedPath(wxFileName savedPath) { m_savedPath = savedPath; }
	void SetJustOpened(bool justOpened) { m_justOpened = justOpened; }
	//virtual void Redraw() { m_glCanvas->Refresh(); }
	virtual void Redraw() { m_workspacePanel->Refresh(); }
	//wxGLContext* GetSharedGLContext() { return m_glContext; }
	void RotateSelectedElements(bool clockwise = true);
	void DeleteSelectedElements();
	bool GetElementsCorners(wxPoint2DDouble& leftUpCorner,
		wxPoint2DDouble& rightDownCorner,
		std::vector<Element*> elementList);
	void Fit();
	void UnselectAll();
	void EnableHeatMap(const bool& enable = true);
	bool IsHeatMapEnable() const { return m_showHM; }
	HMPlane* GetHeatMap() const { return m_hmPlane; }
	void EnableAutoHeatMapLabel(const bool& enable = true) { m_hmAutomaticLabel = enable; }
	bool IsHeatMapAutoLabelEnable() const { return m_hmAutomaticLabel; }


	void ValidateBusesVoltages(Element* initialBus);
	void ValidateElementsVoltages();

	void UpdateElementsID();
	bool UpdateTextElements();
	void UpdateHeatMap();

	int GetElementNumber(ElementID elementID) { return m_elementNumber[elementID]; }
	void IncrementElementNumber(ElementID elementID) { m_elementNumber[elementID]++; }
	PropertiesData* GetProperties() const { return m_properties; }
	std::vector<double> GetStabilityTimeVector() const { return m_stabilityTimeVector; }
	bool IsContinuousCalculationActive() const { return m_continuousCalc; }
	void SetContinuousCalculationActive(bool value = true) { m_continuousCalc = value; }
	bool RunPowerFlow();
	bool RunFault();
	bool RunSCPower();
	bool RunStaticStudies();
	bool RunStability();
	bool RunHarmonicDistortion();
	bool RunFrequencyResponse();

	virtual void OnMiddleDoubleClick(wxMouseEvent& event);
	virtual void OnIdle(wxIdleEvent& event);
	virtual void OnTimer(wxTimerEvent& event);
	virtual void OnLeftDoubleClick(wxMouseEvent& event);
	virtual void OnRightClickDown(wxMouseEvent& event);
	virtual void OnLeftClickUp(wxMouseEvent& event);
	virtual void OnScroll(wxMouseEvent& event);
	virtual void OnMiddleDown(wxMouseEvent& event);
	virtual void OnMiddleUp(wxMouseEvent& event);
	virtual void OnMouseMotion(wxMouseEvent& event);
	virtual void OnKeyDown(wxKeyEvent& event);
	virtual void OnLeftClickDown(wxMouseEvent& event);
	virtual void OnPaint(wxPaintEvent& event);
	virtual void OnPopupClick(wxCommandEvent& event);
	virtual void OnResize(wxSizeEvent& event);

protected:
	virtual void OnHeatMapTime(wxTimerEvent& event);
	
	//void SetViewport();
	void UpdateStatusBar();
	int GetElementNumberFromList(Element* element);
	void GetStateListsCopy(const std::vector<PowerElement*>& elementsList, const std::vector<Text*>& textList, std::vector<PowerElement*>& elementsListCopy, std::vector<Text*>& textListCopy);

	//wxGLContext* m_glContext = nullptr;
	wxStatusBar* m_statusBar = nullptr;
	Camera* m_camera = nullptr;
	wxTipWindow* m_tipWindow = nullptr;
	wxString m_name;

	WorkspaceMode m_mode = WorkspaceMode::MODE_EDIT;

	std::vector<PowerElement*> m_elementList;
	int m_elementNumber[NUM_ELEMENTS];

	std::vector<Text*> m_textList;

	std::vector< std::vector<PowerElement*> > m_elementListState;
	std::vector< std::vector<Text*> > m_textListState;
	int m_currenteState = -1;
	int m_maxStates = 100;

	wxFileName m_savedPath;

	wxRect2DDouble m_selectionRect;
	wxPoint2DDouble m_startSelRect;

	PropertiesData* m_properties = nullptr;

	std::vector<double> m_stabilityTimeVector;

	bool m_continuousCalc = false;
	bool m_disconnectedElement = false;
	bool m_justOpened = false;

	float m_width = 0.0;
	float m_height = 0.0;

	// Modern OpenGL
	HMPlane* m_hmPlane = nullptr;
	Shader* m_basicShader = nullptr;
	Shader* m_hmLabelShader = nullptr;
	Renderer* m_renderer = nullptr;

	//glm::mat4 m_projMatrix;

	bool m_showHM = false;
	bool m_showHMTimer = false;
	bool m_hmAutomaticLabel = false;



#ifdef _DEBUG
	DebugMainFrame* m_debugFrame = nullptr;
#endif
};

#endif  // WORKSPACE_H
