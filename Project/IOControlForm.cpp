#include "IOControlForm.h"

IOControlForm::IOControlForm(wxWindow* parent, IOControl* ioControl) : IOControlFormBase(parent)
{
    SetSize(GetBestSize());
    
    m_parent = parent;
    m_ioControl = ioControl;

    int ioFlags = m_ioControl->GetIOFlags();
    int inChoiceNumber = -1;
    int outChoiceNumber = -1;

    if(ioFlags & IOControl::IN_TERMINAL_VOLTAGE) {
        m_choiceInput->Append(_("Terminal voltage"));
        m_inputFlags.push_back(IOControl::IN_TERMINAL_VOLTAGE);
        if(m_ioControl->GetValue() == IOControl::IN_TERMINAL_VOLTAGE) inChoiceNumber = (int)m_inputFlags.size() - 1;
    }
    if(ioFlags & IOControl::IN_VELOCITY) {
        m_choiceInput->Append(_("Velocity"));
        m_inputFlags.push_back(IOControl::IN_VELOCITY);
        if(m_ioControl->GetValue() == IOControl::IN_VELOCITY) inChoiceNumber = (int)m_inputFlags.size() - 1;
    }
    if(ioFlags & IOControl::IN_ACTIVE_POWER) {
        m_choiceInput->Append(_("Active power"));
        m_inputFlags.push_back(IOControl::IN_ACTIVE_POWER);
        if(m_ioControl->GetValue() == IOControl::IN_ACTIVE_POWER) inChoiceNumber = (int)m_inputFlags.size() - 1;
    }
    if(ioFlags & IOControl::IN_REACTIVE_POWER) {
        m_choiceInput->Append(_("Reactive power"));
        m_inputFlags.push_back(IOControl::IN_REACTIVE_POWER);
        if(m_ioControl->GetValue() == IOControl::IN_REACTIVE_POWER) inChoiceNumber = (int)m_inputFlags.size() - 1;
    }
    if(ioFlags & IOControl::OUT_FIELD_VOLTAGE) {
        m_choiceOutput->Append(_("Field voltage"));
        m_outputFlags.push_back(IOControl::OUT_FIELD_VOLTAGE);
        if(m_ioControl->GetValue() == IOControl::OUT_FIELD_VOLTAGE) outChoiceNumber = (int)m_outputFlags.size() - 1;
    }
    if(ioFlags & IOControl::OUT_MEC_POWER) {
        m_choiceOutput->Append(_("Mechanical power"));
        m_outputFlags.push_back(IOControl::OUT_MEC_POWER);
        if(m_ioControl->GetValue() == IOControl::OUT_MEC_POWER) outChoiceNumber = (int)m_outputFlags.size() - 1;
    }

    if(inChoiceNumber != -1) {
        m_choiceInput->SetSelection(inChoiceNumber);
        m_checkBoxInput->SetValue(true);
        m_checkBoxOutput->SetValue(false);
        m_choiceOutput->Enable(false);
    } else {
        m_choiceOutput->SetSelection(outChoiceNumber);
        m_checkBoxInput->SetValue(false);
        m_checkBoxOutput->SetValue(true);
        m_choiceInput->Enable(false);
    }
}

IOControlForm::~IOControlForm() {}
void IOControlForm::OnOKButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

bool IOControlForm::ValidateData()
{
    if(m_checkBoxInput->GetValue() && m_choiceInput->GetSelection() != -1) {
        m_ioControl->SetValue(m_inputFlags[m_choiceInput->GetSelection()]);
        return true;
    } else if(m_checkBoxOutput->GetValue() && m_choiceOutput->GetSelection() != -1) {
        m_ioControl->SetValue(m_outputFlags[m_choiceOutput->GetSelection()]);
        return true;
    }

    return false;
}

void IOControlForm::OnInputChecked(wxCommandEvent& event)
{
    m_checkBoxInput->SetValue(true);
    m_checkBoxOutput->SetValue(false);
    m_choiceOutput->Enable(false);
    m_choiceInput->Enable(true);
}

void IOControlForm::OnOutputChecked(wxCommandEvent& event)
{
    m_checkBoxOutput->SetValue(true);
    m_checkBoxInput->SetValue(false);
    m_choiceOutput->Enable(true);
    m_choiceInput->Enable(false);
}
