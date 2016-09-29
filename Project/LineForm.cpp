#include "LineForm.h"
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
    switch(data.nominalVoltageUnit)
	{
	    case UNIT_V:
		{
		    nominalVoltageStr += " V";
		}
		break;
	    case UNIT_kV:
		{
		    nominalVoltageStr += " kV";
		}
		break;
	    default:
		break;
	}
	m_staticTextNominalVoltageValue->SetLabel(nominalVoltageStr);
	
	m_textCtrlNominalPower->SetValue(wxString::FromDouble(data.nominalPower));
	switch(data.nominalPowerUnit)
	{
		case UNIT_VA:
		m_choiceResistance->SetSelection(0);
		break;
		case UNIT_kVA:
		m_choiceResistance->SetSelection(1);
		break;
		case UNIT_MVA:
		m_choiceResistance->SetSelection(2);
		break;
		default:
		break;
	}
	
	m_textCtrlResistance->SetValue(wxString::FromDouble(data.resistance));
	switch(data.resistanceUnit)
	{
		case UNIT_PU:
		m_choiceResistance->SetSelection(0);
		break;
		case UNIT_OHM:
		m_choiceResistance->SetSelection(1);
		break;
		case UNIT_OHM_km:
		m_choiceResistance->SetSelection(2);
		break;
		default:
		break;
	}
	
	m_textCtrlReactance->SetValue(wxString::FromDouble(data.indReactance));
	switch(data.indReactanceUnit)
	{
		case UNIT_PU:
		m_choiceResistance->SetSelection(0);
		break;
		case UNIT_OHM:
		m_choiceResistance->SetSelection(1);
		break;
		case UNIT_OHM_km:
		m_choiceResistance->SetSelection(2);
		break;
		default:
		break;
	}
	
	m_textCtrlSusceptance->SetValue(wxString::FromDouble(data.capSusceptance));
	switch(data.capSusceptanceUnit)
	{
		case UNIT_PU:
		m_choiceResistance->SetSelection(0);
		break;
		case UNIT_S:
		m_choiceResistance->SetSelection(1);
		break;
		case UNIT_S_km:
		m_choiceResistance->SetSelection(2);
		break;
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
void LineForm::OnOKButtonClick(wxCommandEvent& event) { EndModal(wxID_OK); }
void LineForm::OnStabilityButtonClick(wxCommandEvent& event) {}
void LineForm::ReplaceStaticTextLabelChar(wxStaticText* staticText, wchar_t newChar)
{
    wxString label = staticText->GetLabel();
    label[label.length() - 2] = newChar;
    staticText->SetLabel(label);
}

bool LineForm::ValidateData()
{
	LineElectricalData data = m_line->GetElectricalData();
	
	//if(!m_line->DoubleFromString(m_parent, m_textCtrlInertia->GetValue(), data.inertia,
	//                                     _("Value entered incorrectly in the field \"Inertia\".")))
}
