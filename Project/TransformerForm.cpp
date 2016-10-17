#include "TransformerForm.h"
#include "SwitchingForm.h"
#include "Transformer.h"

TransformerForm::TransformerForm(wxWindow* parent, Transformer* transformer)
    : TransformerFormBase(parent)
{
    m_choiceResistance->SetString(1, L'\u03A9');
    m_choiceReactance->SetString(1, L'\u03A9');
    
    SetSize(GetBestSize());
    Layout();
    
    m_parent = parent;
    m_transformer = transformer;
    
    TransformerElectricalData data = transformer->GetElectricalData();
    
    m_textCtrlName->SetValue(data.name);
    
    wxString nominalVoltageStr = wxString::FromDouble(data.primaryNominalVoltage);
    switch(data.primaryNominalVoltageUnit) {
        case UNIT_V: {
            nominalVoltageStr += " V";
        } break;
        case UNIT_kV: {
            nominalVoltageStr += " kV";
        } break;
        default:
            break;
    }
    nominalVoltageStr += " / " + wxString::FromDouble(data.secondaryNominalVoltage);
    switch(data.secondaryNominalVoltageUnit) {
        case UNIT_V: {
            nominalVoltageStr += " V";
        } break;
        case UNIT_kV: {
            nominalVoltageStr += " kV";
        } break;
        default:
            break;
    }
    m_staticTextNominalVoltageValue->SetLabel(nominalVoltageStr);

    m_choiceBaseVoltage->SetSelection(data.baseVoltage);
    
    m_textCtrlNominalPower->SetValue(wxString::FromDouble(data.nominalPower));
    switch(data.nominalPowerUnit) {
        case UNIT_VA: {
            m_choiceNominalPower->SetSelection(0);
        } break;
        case UNIT_kVA: {
            m_choiceNominalPower->SetSelection(1);
        } break;
        case UNIT_MVA: {
            m_choiceNominalPower->SetSelection(2);
        } break;
        default:
            break;
    }
    
    m_textCtrlResistance->SetValue(wxString::FromDouble(data.resistance));
    switch(data.resistanceUnit) {
        case UNIT_PU: {
            m_choiceResistance->SetSelection(0);
        } break;
        case UNIT_OHM: {
            m_choiceResistance->SetSelection(1);
        } break;
        default:
            break;
    }

    m_textCtrlReactance->SetValue(wxString::FromDouble(data.indReactance));
    switch(data.indReactanceUnit) {
        case UNIT_PU: {
            m_choiceReactance->SetSelection(0);
        } break;
        case UNIT_OHM: {
            m_choiceReactance->SetSelection(1);
        } break;
        default:
            break;
    }
    
    m_choiceConnection->SetSelection(data.connection);
    
    m_textCtrlTurnRatio->SetValue(wxString::FromDouble(data.turnsRatio));
    m_textCtrlPhaseShift->SetValue(wxString::FromDouble(data.phaseShift));
    
    m_textCtrlZeroResistance->SetValue(wxString::FromDouble(data.zeroResistance));
    m_textCtrlZeroReactance->SetValue(wxString::FromDouble(data.zeroIndReactance));
    m_textCtrlPrimResistance->SetValue(wxString::FromDouble(data.primaryGrndResistance));
    m_textCtrlPrimReactance->SetValue(wxString::FromDouble(data.primaryGrndReactance));
    m_textCtrlSecResistance->SetValue(wxString::FromDouble(data.secondaryGrndResistance));
    m_textCtrlSecReactance->SetValue(wxString::FromDouble(data.secondaryGrndReactance));
}

TransformerForm::~TransformerForm()
{
}

void TransformerForm::OnCancelButtonClick(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}
void TransformerForm::OnOKButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}
void TransformerForm::OnStabilityButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) {
        SwitchingForm swForm(m_parent, m_transformer);
        swForm.SetTitle(_("Transfomer: Switching"));
        swForm.ShowModal();
        EndModal(wxID_OK);
    }
}

bool TransformerForm::ValidateData()
{
    return true;
}
