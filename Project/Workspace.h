#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <wx/dcclient.h>
#include <wx/msgdlg.h>
#include <wx/statusbr.h>
#include <wx/clipbrd.h>
#include <wx/tipwin.h>

#include "WorkspaceBase.h"
#include "Bus.h"

#include "ControlEditor.h"

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
class ElementDataObject;

class Text;

class PowerFlow;
class Fault;
class Electromechanical;

class ElementPlotData;
class ChartView;

class PropertiesData;

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
    ID_TEXT,

    NUM_ELEMENTS
};

class Workspace : public WorkspaceBase
{
public:
    enum WorkspaceMode {
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
    Workspace(wxWindow* parent, wxString name = wxEmptyString, wxStatusBar* statusBar = NULL);
    ~Workspace();

    wxString GetName() const { return m_name; }
    std::vector<Element*> GetElementList() const;
    std::vector<Text*> GetTextList() const { return m_textList; }
    std::vector<Element*> GetAllElements() const;
    WorkspaceMode GetWorkspaceMode() const { return m_mode; }
    Camera* GetCamera() const { return m_camera; }

    void CopySelection();
    bool Paste();

    wxFileName GetSavedPath() const { return m_savedPath; }

    void SetName(wxString name) { m_name = name; }
    void SetElementList(std::vector<Element*> elementList);
    void SetTextList(std::vector<Text*> textList);
    void SetStatusBarText(wxString text) { m_statusBar->SetStatusText(text); }
    void SetWorkspaceMode(WorkspaceMode mode) { m_mode = mode; }
    void SetSavedPath(wxFileName savedPath) { m_savedPath = savedPath; }
    void SetJustOpened(bool justOpened) { m_justOpened = justOpened; }

    void Redraw() { m_glCanvas->Refresh(); }
    void RotateSelectedElements(bool clockwise = true);
    void DeleteSelectedElements();
    bool GetElementsCorners(wxPoint2DDouble& leftUpCorner,
        wxPoint2DDouble& rightDownCorner,
        std::vector<Element*> elementList);
    void Fit();
    void UnselectAll();

    void ValidateBusesVoltages(Element* initialBus);
    void ValidateElementsVoltages();

    void UpdateElementsID();
    void UpdateTextElements();

    int GetElementNumber(ElementID elementID) { return m_elementNumber[elementID]; }
    void IncrementElementNumber(ElementID elementID) { m_elementNumber[elementID]++; }
    
    PropertiesData* GetProperties() const { return m_properties; }
    
    std::vector<double> GetStabilityTimeVector() const { return m_stabilityTimeVector; }

    bool RunPowerFlow();
    bool RunFault();
    bool RunSCPower();
    bool RunStability();

protected:
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

    void SetViewport();
    void UpdateStatusBar();

    wxGLContext* m_glContext = NULL;
    wxStatusBar* m_statusBar = NULL;
    Camera* m_camera = NULL;
    wxTipWindow* m_tipWindow = NULL;
    wxString m_name;

    WorkspaceMode m_mode = MODE_EDIT;

    std::vector<PowerElement*> m_elementList;
    int m_elementNumber[NUM_ELEMENTS];

    std::vector<Text*> m_textList;

    wxFileName m_savedPath;

    wxRect2DDouble m_selectionRect;
    wxPoint2DDouble m_startSelRect;
    
    PropertiesData* m_properties = NULL;
    
    std::vector<double> m_stabilityTimeVector;

    bool m_justOpened = false;
};

#endif // WORKSPACE_H
