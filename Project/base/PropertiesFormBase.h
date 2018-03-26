//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: PropertiesForm.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _PSP_PROJECT_PROPERTIESFORM_BASE_CLASSES_H
#define _PSP_PROJECT_PROPERTIESFORM_BASE_CLASSES_H

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
#include <wx/choice.h>
#include <wx/arrstr.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/checkbox.h>
#include <wx/statbmp.h>
#include <wx/grid.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/hyperlink.h>
#include <wx/filepicker.h>
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


class GeneralPropertiesFormBase : public wxDialog
{
protected:
    wxNotebook* m_notebook;
    wxPanel* m_panelGeneral;
    wxStaticText* m_staticTextLanguage;
    wxChoice* m_choiceLanguage;
    wxStaticText* m_staticTextTheme;
    wxChoice* m_choiceTheme;
    wxButton* m_buttonOK;
    wxButton* m_buttonCancel;

protected:
    virtual void OnButtonOKClick(wxCommandEvent& event) { event.Skip(); }
    virtual void OnButtonCancelClick(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticText* GetStaticTextLanguage() { return m_staticTextLanguage; }
    wxChoice* GetChoiceLanguage() { return m_choiceLanguage; }
    wxStaticText* GetStaticTextTheme() { return m_staticTextTheme; }
    wxChoice* GetChoiceTheme() { return m_choiceTheme; }
    wxPanel* GetPanelGeneral() { return m_panelGeneral; }
    wxNotebook* GetNotebook() { return m_notebook; }
    wxButton* GetButtonOK() { return m_buttonOK; }
    wxButton* GetButtonCancel() { return m_buttonCancel; }
    GeneralPropertiesFormBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("General settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1,-1), long style = wxDEFAULT_DIALOG_STYLE);
    virtual ~GeneralPropertiesFormBase();
};


class SimulationsSettingsFormBase : public wxDialog
{
protected:
    wxNotebook* m_notebook;
    wxPanel* m_panelGeneral;
    wxStaticText* m_staticTextBasePower;
    wxTextCtrl* m_textCtrlbasePower;
    wxChoice* m_choiceBasePower;
    wxCheckBox* m_checkBoxFaultAfterPF;
    wxCheckBox* m_checkBoxSCPowerAfterPF;
    wxPanel* m_panelPF;
    wxStaticText* m_staticTextPFMethod;
    wxChoice* m_choicePFMethod;
    wxStaticText* m_staticTextAccFactor;
    wxTextCtrl* m_textCtrlAccFactor;
    wxStaticText* m_staticTextPFTolerance;
    wxTextCtrl* m_textCtrlPFTolerance;
    wxStaticText* m_staticTextPFMaxIterations;
    wxTextCtrl* m_textCtrlPFMaxIterations;
    wxPanel* m_panelStability;
    wxStaticText* m_staticTextTimeStep;
    wxTextCtrl* m_textCtrlTimeStep;
    wxStaticText* m_staticTextSec_1;
    wxStaticText* m_staticTextTSimTime;
    wxTextCtrl* m_textCtrlSimTime;
    wxStaticText* m_staticTextSec_2;
    wxStaticText* m_staticTextFreq;
    wxTextCtrl* m_textCtrlFreq;
    wxStaticText* m_staticTextFreqUnit;
    wxStaticText* m_staticTextTStabTolerance;
    wxTextCtrl* m_textCtrlStabTolerance;
    wxStaticText* m_staticTextTStabMaxIterations;
    wxTextCtrl* m_textCtrlStabMaxIterations;
    wxStaticText* m_staticTextCtrlStepRation;
    wxTextCtrl* m_textCtrlCtrlStepRatio;
    wxStaticText* m_staticTextPrintTime;
    wxTextCtrl* m_textCtrlPrintTime;
    wxStaticText* m_staticTextSec_4;
    wxCheckBox* m_checkBoxUseCOI;
    wxPanel* m_panelLoadComp;
    wxCheckBox* m_checkBoxUseCompLoads;
    wxStaticText* m_staticTextActivePowerImp;
    wxTextCtrl* m_textCtrlActivePowerImp;
    wxStaticText* m_staticTextPerc_1;
    wxStaticText* m_staticTextActivePowerCur;
    wxTextCtrl* m_textCtrlActivePowerCur;
    wxStaticText* m_staticTextPerc_2;
    wxStaticText* m_staticTextActivePowerPow;
    wxTextCtrl* m_textCtrlActivePowerPow;
    wxStaticText* m_staticTextPerc_3;
    wxStaticText* m_staticTextReactivePowerImp;
    wxTextCtrl* m_textCtrlReactivePowerImp;
    wxStaticText* m_staticTextPerc_4;
    wxStaticText* m_staticTextReactivePowerCur;
    wxTextCtrl* m_textCtrlReactivePowerCur;
    wxStaticText* m_staticTextPerc_5;
    wxStaticText* m_staticTextReactivePowerPow;
    wxTextCtrl* m_textCtrlReactivePowerPow;
    wxStaticText* m_staticTextPerc_6;
    wxStaticText* m_staticTextUV;
    wxStaticText* m_staticTextUVCur;
    wxTextCtrl* m_textCtrlUVCur;
    wxStaticText* m_staticTextPerc_7;
    wxStaticText* m_staticTextUVPow;
    wxTextCtrl* m_textCtrlUVPow;
    wxStaticText* m_staticTextPerc_8;
    wxButton* m_buttonOK;
    wxButton* m_buttonCancel;

protected:
    virtual void OnPFMethodChoiceSelected(wxCommandEvent& event) { event.Skip(); }
    virtual void OnCheckboxUseCompLoadClick(wxCommandEvent& event) { event.Skip(); }
    virtual void OnButtonOKClick(wxCommandEvent& event) { event.Skip(); }
    virtual void OnButtonCancelClick(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticText* GetStaticTextBasePower() { return m_staticTextBasePower; }
    wxTextCtrl* GetTextCtrlbasePower() { return m_textCtrlbasePower; }
    wxChoice* GetChoiceBasePower() { return m_choiceBasePower; }
    wxCheckBox* GetCheckBoxFaultAfterPF() { return m_checkBoxFaultAfterPF; }
    wxCheckBox* GetCheckBoxSCPowerAfterPF() { return m_checkBoxSCPowerAfterPF; }
    wxPanel* GetPanelGeneral() { return m_panelGeneral; }
    wxStaticText* GetStaticTextPFMethod() { return m_staticTextPFMethod; }
    wxChoice* GetChoicePFMethod() { return m_choicePFMethod; }
    wxStaticText* GetStaticTextAccFactor() { return m_staticTextAccFactor; }
    wxTextCtrl* GetTextCtrlAccFactor() { return m_textCtrlAccFactor; }
    wxStaticText* GetStaticTextPFTolerance() { return m_staticTextPFTolerance; }
    wxTextCtrl* GetTextCtrlPFTolerance() { return m_textCtrlPFTolerance; }
    wxStaticText* GetStaticTextPFMaxIterations() { return m_staticTextPFMaxIterations; }
    wxTextCtrl* GetTextCtrlPFMaxIterations() { return m_textCtrlPFMaxIterations; }
    wxPanel* GetPanelPF() { return m_panelPF; }
    wxStaticText* GetStaticTextTimeStep() { return m_staticTextTimeStep; }
    wxTextCtrl* GetTextCtrlTimeStep() { return m_textCtrlTimeStep; }
    wxStaticText* GetStaticTextSec_1() { return m_staticTextSec_1; }
    wxStaticText* GetStaticTextTSimTime() { return m_staticTextTSimTime; }
    wxTextCtrl* GetTextCtrlSimTime() { return m_textCtrlSimTime; }
    wxStaticText* GetStaticTextSec_2() { return m_staticTextSec_2; }
    wxStaticText* GetStaticTextFreq() { return m_staticTextFreq; }
    wxTextCtrl* GetTextCtrlFreq() { return m_textCtrlFreq; }
    wxStaticText* GetStaticTextFreqUnit() { return m_staticTextFreqUnit; }
    wxStaticText* GetStaticTextTStabTolerance() { return m_staticTextTStabTolerance; }
    wxTextCtrl* GetTextCtrlStabTolerance() { return m_textCtrlStabTolerance; }
    wxStaticText* GetStaticTextTStabMaxIterations() { return m_staticTextTStabMaxIterations; }
    wxTextCtrl* GetTextCtrlStabMaxIterations() { return m_textCtrlStabMaxIterations; }
    wxStaticText* GetStaticTextCtrlStepRation() { return m_staticTextCtrlStepRation; }
    wxTextCtrl* GetTextCtrlCtrlStepRatio() { return m_textCtrlCtrlStepRatio; }
    wxStaticText* GetStaticTextPrintTime() { return m_staticTextPrintTime; }
    wxTextCtrl* GetTextCtrlPrintTime() { return m_textCtrlPrintTime; }
    wxStaticText* GetStaticTextSec_4() { return m_staticTextSec_4; }
    wxCheckBox* GetCheckBoxUseCOI() { return m_checkBoxUseCOI; }
    wxPanel* GetPanelStability() { return m_panelStability; }
    wxCheckBox* GetCheckBoxUseCompLoads() { return m_checkBoxUseCompLoads; }
    wxStaticText* GetStaticTextActivePowerImp() { return m_staticTextActivePowerImp; }
    wxTextCtrl* GetTextCtrlActivePowerImp() { return m_textCtrlActivePowerImp; }
    wxStaticText* GetStaticTextPerc_1() { return m_staticTextPerc_1; }
    wxStaticText* GetStaticTextActivePowerCur() { return m_staticTextActivePowerCur; }
    wxTextCtrl* GetTextCtrlActivePowerCur() { return m_textCtrlActivePowerCur; }
    wxStaticText* GetStaticTextPerc_2() { return m_staticTextPerc_2; }
    wxStaticText* GetStaticTextActivePowerPow() { return m_staticTextActivePowerPow; }
    wxTextCtrl* GetTextCtrlActivePowerPow() { return m_textCtrlActivePowerPow; }
    wxStaticText* GetStaticTextPerc_3() { return m_staticTextPerc_3; }
    wxStaticText* GetStaticTextReactivePowerImp() { return m_staticTextReactivePowerImp; }
    wxTextCtrl* GetTextCtrlReactivePowerImp() { return m_textCtrlReactivePowerImp; }
    wxStaticText* GetStaticTextPerc_4() { return m_staticTextPerc_4; }
    wxStaticText* GetStaticTextReactivePowerCur() { return m_staticTextReactivePowerCur; }
    wxTextCtrl* GetTextCtrlReactivePowerCur() { return m_textCtrlReactivePowerCur; }
    wxStaticText* GetStaticTextPerc_5() { return m_staticTextPerc_5; }
    wxStaticText* GetStaticTextReactivePowerPow() { return m_staticTextReactivePowerPow; }
    wxTextCtrl* GetTextCtrlReactivePowerPow() { return m_textCtrlReactivePowerPow; }
    wxStaticText* GetStaticTextPerc_6() { return m_staticTextPerc_6; }
    wxStaticText* GetStaticTextUV() { return m_staticTextUV; }
    wxStaticText* GetStaticTextUVCur() { return m_staticTextUVCur; }
    wxTextCtrl* GetTextCtrlUVCur() { return m_textCtrlUVCur; }
    wxStaticText* GetStaticTextPerc_7() { return m_staticTextPerc_7; }
    wxStaticText* GetStaticTextUVPow() { return m_staticTextUVPow; }
    wxTextCtrl* GetTextCtrlUVPow() { return m_textCtrlUVPow; }
    wxStaticText* GetStaticTextPerc_8() { return m_staticTextPerc_8; }
    wxPanel* GetPanelLoadComp() { return m_panelLoadComp; }
    wxNotebook* GetNotebook() { return m_notebook; }
    wxButton* GetButtonOK() { return m_buttonOK; }
    wxButton* GetButtonCancel() { return m_buttonCancel; }
    SimulationsSettingsFormBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Simulation settings"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1,-1), long style = wxDEFAULT_DIALOG_STYLE);
    virtual ~SimulationsSettingsFormBase();
};


class AboutFormBase : public wxDialog
{
protected:
    wxNotebook* m_notebook;
    wxPanel* m_panelLogo;
    wxStaticBitmap* m_staticBitmapLogo;
    wxPanel* m_panelCredits;
    wxGrid* m_gridCredits;
    wxPanel* m_panelLicense;
    wxRichTextCtrl* m_richTextCtrlLicense;
    wxStaticText* m_staticTextVersionLabel;
    wxStaticText* m_staticTextVersion;
    wxStaticText* m_staticTextHome;
    wxHyperlinkCtrl* m_hyperLinkPSP;
    wxButton* m_buttonOK;

protected:
    virtual void OnOKButtonClick(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticBitmap* GetStaticBitmapLogo() { return m_staticBitmapLogo; }
    wxPanel* GetPanelLogo() { return m_panelLogo; }
    wxGrid* GetGridCredits() { return m_gridCredits; }
    wxPanel* GetPanelCredits() { return m_panelCredits; }
    wxRichTextCtrl* GetRichTextCtrlLicense() { return m_richTextCtrlLicense; }
    wxPanel* GetPanelLicense() { return m_panelLicense; }
    wxNotebook* GetNotebook() { return m_notebook; }
    wxStaticText* GetStaticTextVersionLabel() { return m_staticTextVersionLabel; }
    wxStaticText* GetStaticTextVersion() { return m_staticTextVersion; }
    wxStaticText* GetStaticTextHome() { return m_staticTextHome; }
    wxHyperlinkCtrl* GetHyperLinkPSP() { return m_hyperLinkPSP; }
    wxButton* GetButtonOK() { return m_buttonOK; }
    AboutFormBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("About PSP-UFU"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1,-1), long style = wxDEFAULT_DIALOG_STYLE);
    virtual ~AboutFormBase();
};


class ImportFormBase : public wxDialog
{
protected:
    wxNotebook* m_notebook;
    wxPanel* m_panelCEPEL;
    wxStaticText* m_staticTextBasePWFFile;
    wxFilePickerCtrl* m_filePickerANAREDEPWF;
    wxStaticText* m_staticTextBaseLSTFile;
    wxFilePickerCtrl* m_filePickerANAREDELST;
    wxStaticText* m_staticTextBaseSTBFile;
    wxFilePickerCtrl* m_filePickerANATEMSTB;
    wxButton* m_buttonOK;
    wxButton* m_buttonCancel;

protected:
    virtual void OnButtonOKClick(wxCommandEvent& event) { event.Skip(); }
    virtual void OnButtonCancelClick(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticText* GetStaticTextBasePWFFile() { return m_staticTextBasePWFFile; }
    wxFilePickerCtrl* GetFilePickerANAREDEPWF() { return m_filePickerANAREDEPWF; }
    wxStaticText* GetStaticTextBaseLSTFile() { return m_staticTextBaseLSTFile; }
    wxFilePickerCtrl* GetFilePickerANAREDELST() { return m_filePickerANAREDELST; }
    wxStaticText* GetStaticTextBaseSTBFile() { return m_staticTextBaseSTBFile; }
    wxFilePickerCtrl* GetFilePickerANATEMSTB() { return m_filePickerANATEMSTB; }
    wxPanel* GetPanelCEPEL() { return m_panelCEPEL; }
    wxNotebook* GetNotebook() { return m_notebook; }
    wxButton* GetButtonOK() { return m_buttonOK; }
    wxButton* GetButtonCancel() { return m_buttonCancel; }
    ImportFormBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Import files"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(-1,-1), long style = wxDEFAULT_DIALOG_STYLE);
    virtual ~ImportFormBase();
};

#endif
