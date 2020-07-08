#include "ProjectPropertiesForm.h"
#include "Workspace.h"
#include "HMPlane.h"

ProjectPropertiesForm::ProjectPropertiesForm(wxWindow* parent, Workspace* workspace)
    : ProjectPropertiesFormBase(parent), m_workspace(workspace)
{
    m_checkBoxAutomaticLabel->SetValue(m_workspace->IsHeatMapAutoLabelEnable());
    m_textCtrlMaxVoltage->SetValue(wxString::Format("%f", m_workspace->GetHeatMap()->GetMaxLimit()));
    m_textCtrlMinVoltage->SetValue(wxString::Format("%f", m_workspace->GetHeatMap()->GetMinLimit()));

    EnableLabelLimits(!m_checkBoxAutomaticLabel->GetValue());
}

void ProjectPropertiesForm::OnAutomaticLabelClick(wxCommandEvent& event)
{
    EnableLabelLimits(!m_checkBoxAutomaticLabel->GetValue());
}

bool ProjectPropertiesForm::ValidateData()
{
    if(m_checkBoxAutomaticLabel->GetValue()) {
        m_workspace->EnableAutoHeatMapLabel();
    }
    else {
        m_workspace->EnableAutoHeatMapLabel(false);
        double maxVoltage, minVoltage;

        if (!Element::DoubleFromString(this, m_textCtrlMaxVoltage->GetValue(), maxVoltage,
            _("Value entered incorrectly in the field \"Label max voltage\".")))
            return false;
        if (!Element::DoubleFromString(this, m_textCtrlMinVoltage->GetValue(), minVoltage,
            _("Value entered incorrectly in the field \"Label min voltage\".")))
            return false;

        m_workspace->GetHeatMap()->SetLabelLimits(static_cast<float>(minVoltage), static_cast<float>(maxVoltage));
    }
    return true;
}

void ProjectPropertiesForm::EnableLabelLimits(const bool& enable)
{
    m_textCtrlMaxVoltage->Enable(enable);
    m_textCtrlMinVoltage->Enable(enable);
    m_staticTextMaxVoltage->Enable(enable);
    m_staticTextMinVoltage->Enable(enable);
    m_staticTextPU_1->Enable(enable);
    m_staticTextPU_2->Enable(enable);
}
