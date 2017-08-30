#include "SimulationsSettingsForm.h"
#include "PropertiesData.h"

SimulationsSettingsForm::SimulationsSettingsForm(wxWindow* parent, PropertiesData* properties)
    : SimulationsSettingsFormBase(parent)
{
    m_properties = properties;
    auto data = m_properties->GetSimulationPropertiesData();

    m_textCtrlbasePower->SetValue(Element::StringFromDouble(data.basePower));
    switch(data.basePowerUnit) {
        case UNIT_VA: {
            m_choiceBasePower->SetSelection(0);
        } break;
        case UNIT_kVA: {
            m_choiceBasePower->SetSelection(1);
        } break;
        case UNIT_MVA: {
            m_choiceBasePower->SetSelection(2);
        } break;
        default: {
            m_choiceBasePower->SetSelection(wxNOT_FOUND);
        } break;
    }
    m_checkBoxFaultAfterPF->SetValue(data.faultAfterPowerFlow);
    m_checkBoxSCPowerAfterPF->SetValue(data.scPowerAfterPowerFlow);
    switch(data.powerFlowMethod) {
        case GAUSS_SEIDEL: {
            m_choicePFMethod->SetSelection(0);
        } break;
        case NEWTON_RAPHSON: {
            m_choicePFMethod->SetSelection(1);
            m_textCtrlAccFactor->Enable(false);
        } break;
        default: {
            m_choicePFMethod->SetSelection(wxNOT_FOUND);
        } break;
    }
    m_textCtrlAccFactor->SetValue(Element::StringFromDouble(data.accFator));
    m_textCtrlPFTolerance->SetValue(wxString::Format("%g", data.powerFlowTolerance));
    m_textCtrlPFMaxIterations->SetValue(wxString::Format("%d", data.powerFlowMaxIterations));
    m_textCtrlTimeStep->SetValue(wxString::Format("%g", data.timeStep));
    m_textCtrlSimTime->SetValue(Element::StringFromDouble(data.stabilitySimulationTime));
    m_textCtrlFreq->SetValue(Element::StringFromDouble(data.stabilityFrequency));
    m_textCtrlStabTolerance->SetValue(wxString::Format("%g", data.stabilityTolerance));
    m_textCtrlStabMaxIterations->SetValue(wxString::Format("%d", data.stabilityMaxIterations));
    m_textCtrlCtrlStepRatio->SetValue(wxString::Format("%d", data.controlTimeStepRatio));
    m_textCtrlPrintTime->SetValue(wxString::Format("%g", data.plotTime));

    m_checkBoxUseCOI->SetValue(data.useCOI);
}

SimulationsSettingsForm::~SimulationsSettingsForm() {}
void SimulationsSettingsForm::OnButtonOKClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

bool SimulationsSettingsForm::ValidateData()
{
    auto data = m_properties->GetSimulationPropertiesData();
    if(!Element::DoubleFromString(this, m_textCtrlbasePower->GetValue(), data.basePower,
                                  _("Value entered incorrectly in the field \"Base power\".")))
        return false;
    switch(m_choiceBasePower->GetSelection()) {
        case 0: {
            data.basePowerUnit = UNIT_VA;
        } break;
        case 1: {
            data.basePowerUnit = UNIT_kVA;
        } break;
        default: {
            data.basePowerUnit = UNIT_MVA;
        } break;
    }
    data.faultAfterPowerFlow = m_checkBoxFaultAfterPF->GetValue();
    data.scPowerAfterPowerFlow = m_checkBoxSCPowerAfterPF->GetValue();
    switch(m_choicePFMethod->GetSelection()) {
        case 0: {
            data.powerFlowMethod = GAUSS_SEIDEL;
        } break;
        case 1: {
            data.powerFlowMethod = NEWTON_RAPHSON;
        } break;
    }
    if(!Element::DoubleFromString(this, m_textCtrlAccFactor->GetValue(), data.accFator,
                                  _("Value entered incorrectly in the field \"Acceleration factor\".")))
        return false;
    if(!Element::DoubleFromString(this, m_textCtrlPFTolerance->GetValue(), data.powerFlowTolerance,
                                  _("Value entered incorrectly in the field \"Tolerance (Power flow)\".")))
        return false;
    if(!Element::IntFromString(this, m_textCtrlPFMaxIterations->GetValue(), data.powerFlowMaxIterations,
                               _("Value entered incorrectly in the field \"Max. iterations (Power flow)\".")))
        return false;
    if(!Element::DoubleFromString(this, m_textCtrlTimeStep->GetValue(), data.timeStep,
                                  _("Value entered incorrectly in the field \"Time step\".")))
        return false;
    if(!Element::DoubleFromString(this, m_textCtrlSimTime->GetValue(), data.stabilitySimulationTime,
                                  _("Value entered incorrectly in the field \"Simulation time\".")))
        return false;
    if(!Element::DoubleFromString(this, m_textCtrlFreq->GetValue(), data.stabilityFrequency,
                                  _("Value entered incorrectly in the field \"System frequency\".")))
        return false;
    if(!Element::DoubleFromString(this, m_textCtrlStabTolerance->GetValue(), data.stabilityTolerance,
                                  _("Value entered incorrectly in the field \"Tolerance (Stability)\".")))
        return false;
    if(!Element::IntFromString(this, m_textCtrlStabMaxIterations->GetValue(), data.stabilityMaxIterations,
                               _("Value entered incorrectly in the field \"Max. iterations (Stability)\".")))
        return false;
    if(!Element::IntFromString(this, m_textCtrlCtrlStepRatio->GetValue(), data.controlTimeStepRatio,
                               _("Value entered incorrectly in the field \"Controls step ratio\".")))
        return false;
    if(!Element::DoubleFromString(this, m_textCtrlPrintTime->GetValue(), data.plotTime,
                                  _("Value entered incorrectly in the field \"Plot time\".")))
        return false;
    data.useCOI = m_checkBoxUseCOI->GetValue();

    m_properties->SetSimulationPropertiesData(data);
    return true;
}
void SimulationsSettingsForm::OnPFMethodChoiceSelected(wxCommandEvent& event)
{
    if(m_choicePFMethod->GetSelection() == 0)
        m_textCtrlAccFactor->Enable();
    else
        m_textCtrlAccFactor->Enable(false);
}
