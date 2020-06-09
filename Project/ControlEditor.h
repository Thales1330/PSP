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

#ifndef CONTROLEDITOR_H
#define CONTROLEDITOR_H

#include "ControlEditorBase.h"
#include <wx/wrapsizer.h>
#include <wx/dcclient.h>
#include <wx/dcscreen.h>
#include <wx/graphics.h>
#ifdef _MSC_VER
#include <windows.h>
#endif 
#include <GL/gl.h>
#include <GL/glu.h>

#include <wx/progdlg.h>

#include "IOControl.h"
#include "ControlSystemTest.h"

class FileHanding;
class Camera;
class Element;
class ControlElement;
class TransferFunction;
class ConnectionLine;
class Sum;
class Multiplier;
class Limiter;
class RateLimiter;
class Exponential;
class Constant;
class Gain;
class MathOperation;
class Divider;
class MathExpression;

class ControlElementSolver;
class ControlElementContainer;

class ChartView;
class ElementDataObject;

enum class ControlElementButtonID : int {
    ID_IO = 0,
    ID_TF,
    ID_SUM,
    ID_CONST,
    ID_LIMITER,
    ID_GAIN,
    ID_MULT,
    ID_EXP,
    ID_RATELIM,
    ID_MATH_DIV,
    ID_MATH_EXPR
};

/**
 * @class ControlElementButton
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief This class is responsible to handle the user interaction with control elements.
 * @file ControlEditor.h
 */
class ControlElementButton : public wxWindow
{
   public:
    ControlElementButton(wxWindow* parent, wxString label, wxImage image, wxWindowID id = wxID_ANY);
    ~ControlElementButton();

   protected:
    virtual void OnPaint(wxPaintEvent& event);
    virtual void OnMouseEnter(wxMouseEvent& event);
    virtual void OnMouseLeave(wxMouseEvent& event);
    virtual void OnLeftClickDown(wxMouseEvent& event);
    virtual void OnLeftClickUp(wxMouseEvent& event);

    wxString m_label;
    wxFont m_font;
    wxPoint m_labelPosition;

    wxImage m_image;
    wxSize m_imageSize;
    wxPoint m_imagePosition;

    int m_borderSize = 2;
    bool m_mouseAbove = false;
    bool m_selected = false;

    wxSize m_buttonSize;
};

class ControlEditor : public ControlEditorBase
{
   public:
    enum class ControlEditorMode : int {
        MODE_EDIT = 0,
        MODE_MOVE_ELEMENT,
        MODE_MOVE_LINE,
        MODE_DRAG,
        MODE_DRAG_INSERT,
        MODE_INSERT,
        MODE_INSERT_LINE,
        MODE_SELECTION_RECT,
        MODE_PASTE,
        MODE_DRAG_PASTE
    };

    ControlEditor(wxWindow* parent, wxGLContext* sharedGLContext,
                  int ioflags = IOControl::IN_TERMINAL_VOLTAGE | IOControl::IN_VELOCITY | IOControl::OUT_FIELD_VOLTAGE |
                                IOControl::OUT_MEC_POWER);
    virtual ~ControlEditor();

    virtual void AddElement(ControlElementButtonID id);
    virtual void Redraw() { m_glCanvas->Refresh(); }
    virtual void SetJustOpened(bool justOpened) { m_justOpened = justOpened; }
    virtual void RotateSelectedElements(bool clockwise);
    virtual void DeleteSelectedElements();
    virtual void CheckConnections();
    virtual std::vector<ConnectionLine*> GetConnectionLineList() const { return m_connectionList; }
    virtual std::vector<ControlElement*> GetControlElementList() const { return m_elementList; }
    virtual void SetElementsList(std::vector<ControlElement*> elementList) { m_elementList = elementList; }
    virtual void SetConnectionsList(std::vector<ConnectionLine*> connectionList) { m_connectionList = connectionList; }
    virtual void SetControlContainer(ControlElementContainer* ctrlContainer) { m_ctrlContainer = ctrlContainer; }

    virtual void OnClose(wxCloseEvent& event);
    virtual void OnTestClick(wxCommandEvent& event);
    virtual void OnButtonOKClick(wxCommandEvent& event) { Close(); }
    virtual void OnImportClick(wxCommandEvent& event);
    virtual void OnExportClick(wxCommandEvent& event);
    virtual void OnKeyDown(wxKeyEvent& event);
    virtual void OnIdle(wxIdleEvent& event);
    virtual void OnScroll(wxMouseEvent& event);
    virtual void OnDoubleClick(wxMouseEvent& event);
    virtual void OnLeftClickDown(wxMouseEvent& event);
    virtual void OnLeftClickUp(wxMouseEvent& event);
    virtual void OnMiddleDown(wxMouseEvent& event);
    virtual void OnMiddleUp(wxMouseEvent& event);
    virtual void OnMouseMotion(wxMouseEvent& event);
    virtual void OnPaint(wxPaintEvent& event);
    virtual void LeftClickDown(wxMouseEvent& event);
    
    void BuildControlElementPanel();

   protected:
    void SetViewport();
    int GetNextID();

    std::vector<ConnectionLine*>::iterator DeleteLineFromList(std::vector<ConnectionLine*>::iterator& it);

    wxGLContext* m_glContext = NULL;
    Camera* m_camera = NULL;

    ControlEditorMode m_mode = ControlEditorMode::MODE_EDIT;

    wxRect2DDouble m_selectionRect;
    wxPoint2DDouble m_startSelRect;

    std::vector<ControlElement*> m_elementList;
    std::vector<ConnectionLine*> m_connectionList;

    ControlElementContainer* m_ctrlContainer = NULL;

    bool m_justOpened = false;
    int m_ioFlags;

    int m_inputType = 0;
    double m_startTime = 1.0;
    double m_slope = 1.0;
    double m_timeStep = 1e-4;
    double m_simTime = 10.0;
};
#endif  // CONTROLEDITOR_H
