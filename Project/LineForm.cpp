#include "LineForm.h"
#include "SwitchingForm.h"
#include "Line.h"

LineForm::LineForm(wxWindow* parent, Line* line) : LineFormBase(parent)
{
    m_choiceResistance->SetString(1, L'\u03A9');
    m_choiceResistance->SetString(2, (wxString)L'\u03A9' + "/km");
    m_choiceResistance->SetInitialSize();
    m_textCtrlResistance->SetInitialSize();

    m_choiceReactance->SetString(1, L'\u03A9');
    m_choiceReactance->SetString(2, (wxString)L'\u03A9' + "/km");
    m_choiceReactance->SetInitialSize();
    m_textCtrlReactance->SetInitialSize();

    ReplaceStaticTextLabelChar(m_staticTextZeroResistance, L'\u2080');
    ReplaceStaticTextLabelChar(m_staticTextZeroReactance, L'\u2080');
    ReplaceStaticTextLabelChar(m_staticTextZeroSusceptance, L'\u2080');

    SetSize(GetBestSize());
    Layout();
    m_parent = parent;
    m_line = line;

    LineElectricalData data = line->GetElectricalData();

    m_textCtrlName->SetValue(data.name);

    wxString nominalVoltageStr = wxString::FromDouble(data.nominalVoltage);
    switch(data.nominalVoltageUnit) {
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
        case UNIT_OHM_km: {
            m_choiceResistance->SetSelection(2);
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
        case UNIT_OHM_km: {
            m_choiceReactance->SetSelection(2);
        } break;
        default:
            break;
    }

    m_textCtrlSusceptance->SetValue(wxString::FromDouble(data.capSusceptance));
    switch(data.capSusceptanceUnit) {
        case UNIT_PU: {
            m_choiceSusceptance->SetSelection(0);
        } break;
        case UNIT_S: {
            m_choiceSusceptance->SetSelection(1);
        } break;
        case UNIT_S_km: {
            m_choiceSusceptance->SetSelection(2);
        } break;
        default:
            break;
    }

    m_textCtrlLineSize->SetValue(wxString::FromDouble(data.lineSize));
    m_checkUseLinePower->SetValue(data.useLinePower);

    m_textCtrlZeroResistance->SetValue(wxString::FromDouble(data.zeroResistance));
    m_textCtrlZeroReactance->SetValue(wxString::FromDouble(data.zeroIndReactance));
    m_textCtrlZeroSusceptance->SetValue(wxString::FromDouble(data.zeroCapSusceptance));
}

LineForm::~LineForm() {}
void LineForm::OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
void LineForm::OnOKButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

void LineForm::OnStabilityButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) {
        SwitchingForm swForm(m_parent);
        swForm.SetTitle(_("Line: Switching"));
        swForm.ShowModal();
        EndModal(wxID_OK);
    }
}

void LineForm::ReplaceStaticTextLabelChar(wxStaticText* staticText, wchar_t newChar)
{
    wxString label = staticText->GetLabel();
    label[label.length() - 2] = newChar;
    staticText->SetLabel(label);
}

bool LineForm::ValidateData()
{
    LineElectricalData data = m_line->GetElectricalData();

    data.name = m_textCtrlName->GetValue();

    if(!m_line->DoubleFromString(m_parent, m_textCtrlNominalPower->GetValue(), data.nominalPower,
                                 _("Value entered incorrectly in the field \"Nominal power\".")))
        return false;
    switch(m_choiceNominalPower->GetSelection()) {
        case 0: {
            data.nominalPowerUnit = UNIT_VA;
        } break;
        case 1: {
            data.nominalPowerUnit = UNIT_kVA;
        } break;
        case 2: {
            data.nominalPowerUnit = UNIT_MVA;
        } break;
    }

    if(!m_line->DoubleFromString(m_parent, m_textCtrlResistance->GetValue(), data.resistance,
                                 _("Value entered incorrectly in the field \"Resistance\".")))
        return false;
    switch(m_choiceResistance->GetSelection()) {
        case 0: {
            data.resistanceUnit = UNIT_PU;
        } break;
        case 1: {
            data.resistanceUnit = UNIT_OHM;
        } break;
        case 2: {
            data.resistanceUnit = UNIT_OHM_km;
        } break;
    }

    if(!m_line->DoubleFromString(m_parent, m_textCtrlReactance->GetValue(), data.indReactance,
                                 _("Value entered incorrectly in the field \"Indutive Reactance\".")))
        return false;
    switch(m_choiceReactance->GetSelection()) {
        case 0: {
            data.indReactanceUnit = UNIT_PU;
        } break;
        case 1: {
            data.indReactanceUnit = UNIT_OHM;
        } break;
        case 2: {
            data.indReactanceUnit = UNIT_OHM_km;
        } break;
    }

    if(!m_line->DoubleFromString(m_parent, m_textCtrlSusceptance->GetValue(), data.capSusceptance,
                                 _("Value entered incorrectly in the field \"Capacitive Susceptance\".")))
        return false;
    switch(m_choiceSusceptance->GetSelection()) {
        case 0: {
            data.capSusceptanceUnit = UNIT_PU;
        } break;
        case 1: {
            data.capSusceptanceUnit = UNIT_S;
        } break;
        case 2: {
            data.capSusceptanceUnit = UNIT_S_km;
        } break;
    }

    if(!m_line->DoubleFromString(m_parent, m_textCtrlLineSize->GetValue(), data.lineSize,
                                 _("Value entered incorrectly in the field \"Line size\".")))
        return false;

    data.useLinePower = m_checkUseLinePower->GetValue();

    if(!m_line->DoubleFromString(m_parent, m_textCtrlZeroResistance->GetValue(), data.zeroResistance,
                                 _("Value entered incorrectly in the field \"Zero-sequence resistance\".")))
        return false;
    if(!m_line->DoubleFromString(m_parent, m_textCtrlZeroReactance->GetValue(), data.zeroIndReactance,
                                 _("Value entered incorrectly in the field \"Zero-sequence indutive reactance\".")))
        return false;
    if(!m_line->DoubleFromString(m_parent, m_textCtrlZeroSusceptance->GetValue(), data.zeroCapSusceptance,
                                 _("Value entered incorrectly in the field \"Zero-sequence capacitive susceptance\".")))
        return false;

    m_line->SetElectricalData(data);

    return true;
}
