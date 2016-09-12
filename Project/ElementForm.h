//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: ElementForm.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef PSP_PROJECT_ELEMENTFORM_BASE_CLASSES_H
#define PSP_PROJECT_ELEMENTFORM_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/imaglist.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/arrstr.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif


class BusFormBase : public wxDialog
{
protected:
    wxNotebook* m_notebook;
    wxPanel* m_panelGeneral;
    wxStaticText* m_staticTextName;
    wxTextCtrl* m_textCtrlName;
    wxStaticText* m_staticTextNomVoltage;
    wxTextCtrl* m_textCtrlNomVoltage;
    wxChoice* m_choiceNomVoltage;
    wxCheckBox* m_checkBoxCtrlVoltage;
    wxTextCtrl* m_textCtrlCtrlVoltage;
    wxChoice* m_choiceCtrlVoltage;
    wxCheckBox* m_checkBoxSlackBus;
    wxPanel* m_panelFault;
    wxCheckBox* m_checkBoxFault;
    wxStaticText* m_staticTextFaultType;
    wxChoice* m_choiceFaultType;
    wxStaticText* m_staticTextFaultPlace;
    wxChoice* m_choiceFaultPlace;
    wxStaticText* m_staticTextFaultResistance;
    wxTextCtrl* m_textCtrlFaultResistance;
    wxStaticText* m_staticTextPU_1;
    wxStaticText* m_staticTextReactance;
    wxTextCtrl* m_textCtrlFaultReactance;
    wxStaticText* m_staticTextPU_2;
    wxButton* m_buttonOK;
    wxButton* m_ButtonCancel;

protected:
    virtual void OnButtonOKClick(wxCommandEvent& event) { event.Skip(); }
    virtual void OnButtonCancelClick(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticText* GetStaticTextName() { return m_staticTextName; }
    wxTextCtrl* GetTextCtrlName() { return m_textCtrlName; }
    wxStaticText* GetStaticTextNomVoltage() { return m_staticTextNomVoltage; }
    wxTextCtrl* GetTextCtrlNomVoltage() { return m_textCtrlNomVoltage; }
    wxChoice* GetChoiceNomVoltage() { return m_choiceNomVoltage; }
    wxCheckBox* GetCheckBoxCtrlVoltage() { return m_checkBoxCtrlVoltage; }
    wxTextCtrl* GetTextCtrlCtrlVoltage() { return m_textCtrlCtrlVoltage; }
    wxChoice* GetChoiceCtrlVoltage() { return m_choiceCtrlVoltage; }
    wxCheckBox* GetCheckBoxSlackBus() { return m_checkBoxSlackBus; }
    wxPanel* GetPanelGeneral() { return m_panelGeneral; }
    wxCheckBox* GetCheckBoxFault() { return m_checkBoxFault; }
    wxStaticText* GetStaticTextFaultType() { return m_staticTextFaultType; }
    wxChoice* GetChoiceFaultType() { return m_choiceFaultType; }
    wxStaticText* GetStaticTextFaultPlace() { return m_staticTextFaultPlace; }
    wxChoice* GetChoiceFaultPlace() { return m_choiceFaultPlace; }
    wxStaticText* GetStaticTextFaultResistance() { return m_staticTextFaultResistance; }
    wxTextCtrl* GetTextCtrlFaultResistance() { return m_textCtrlFaultResistance; }
    wxStaticText* GetStaticTextPU_1() { return m_staticTextPU_1; }
    wxStaticText* GetStaticTextReactance() { return m_staticTextReactance; }
    wxTextCtrl* GetTextCtrlFaultReactance() { return m_textCtrlFaultReactance; }
    wxStaticText* GetStaticTextPU_2() { return m_staticTextPU_2; }
    wxPanel* GetPanelFault() { return m_panelFault; }
    wxNotebook* GetNotebook() { return m_notebook; }
    wxButton* GetButtonOK() { return m_buttonOK; }
    wxButton* GetButtonCancel() { return m_ButtonCancel; }
    BusFormBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Bus"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1,-1), long style = wxDEFAULT_DIALOG_STYLE);
    virtual ~BusFormBase();
};

#endif