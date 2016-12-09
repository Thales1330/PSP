#include "TextForm.h"

TextForm::TextForm(wxWindow* parent, Text* text, std::vector<Element*> elementList, double systemPowerBase)
    : TextFormBase(parent)
{
    m_parent = parent;
    m_textToEdit = text;
    m_allElements.GetElementsFromList(elementList);
    m_systemPowerBase = systemPowerBase;

    m_text = new Text();
    m_text->SetElementType(text->GetElementType());
    m_text->SetElementNumber(text->GetElementNumber());
    m_text->SetElement(text->GetElement());
    m_text->SetDataType(text->GetDataType());
    m_text->SetDirection(text->GetDirection());
    m_text->SetUnit(text->GetUnit());
    m_text->SetDecimalPlaces(text->GetDecimalPlaces());

    if(!LoadChoices()) {
        m_choiceName->Enable(false);
        m_choiceTextType->Enable(false);
        m_choiceTextFromBus->Enable(false);
        m_choiceTextToBus->Enable(false);
        m_choiceTextUnit->Enable(false);
    }
}

TextForm::~TextForm() {}
void TextForm::OnElementChoiceSelected(wxCommandEvent& event)
{
    switch(m_choiceElement->GetSelection()) {
        case 0: {
            m_text->SetElementType(TYPE_BUS);
        } break;
        case 1: {
            m_text->SetElementType(TYPE_SYNC_GENERATOR);
        } break;
        case 2: {
            m_text->SetElementType(TYPE_LINE);
        } break;
        case 3: {
            m_text->SetElementType(TYPE_TRANSFORMER);
        } break;
        case 4: {
            m_text->SetElementType(TYPE_LOAD);
        } break;
        case 5: {
            m_text->SetElementType(TYPE_CAPACITOR);
        } break;
        case 6: {
            m_text->SetElementType(TYPE_INDUCTOR);
        } break;
        case 7: {
            m_text->SetElementType(TYPE_SYNC_MOTOR);
        } break;
        case 8: {
            m_text->SetElementType(TYPE_IND_MOTOR);
        } break;

        default:
            break;
    }

    ElementTypeChoice();
}

void TextForm::OnFromBusChoiceSelected(wxCommandEvent& event)
{
    m_text->SetDirection(m_choiceTextFromBus->GetSelection());
    m_choiceTextToBus->SetSelection(m_choiceTextFromBus->GetSelection());
}

void TextForm::OnNameChoiceSelected(wxCommandEvent& event)
{
    m_text->SetElementNumber(m_choiceName->GetSelection());
    ElementNumberChoice();
}

void TextForm::OnTextEnter(wxCommandEvent& event) { Preview(); }
void TextForm::OnToBusChoiceSelected(wxCommandEvent& event)
{
    m_text->SetDirection(m_choiceTextToBus->GetSelection());
    m_choiceTextFromBus->SetSelection(m_choiceTextToBus->GetSelection());
}

void TextForm::OnUnitChoiceSelected(wxCommandEvent& event)
{
    UnitChoice();
    Preview();
}

void TextForm::OnTypeChoiceSelected(wxCommandEvent& event)
{
    switch(m_text->GetElementType()) {
        case TYPE_BUS: {
            switch(m_choiceTextType->GetSelection()) {
                case 0: {
                    m_text->SetDataType(DATA_NAME);
                } break;
                case 1: {
                    m_text->SetDataType(DATA_VOLTAGE);
                } break;
                case 2: {
                    m_text->SetDataType(DATA_ANGLE);
                } break;
                case 3: {
                    m_text->SetDataType(DATA_SC_CURRENT);
                } break;
                case 4: {
                    m_text->SetDataType(DATA_SC_VOLTAGE);
                } break;
                case 5: {
                    m_text->SetDataType(DATA_SC_POWER);
                } break;
            }
        } break;
        case TYPE_SYNC_GENERATOR: {
            switch(m_choiceTextType->GetSelection()) {
                case 0: {
                    m_text->SetDataType(DATA_NAME);
                } break;
                case 1: {
                    m_text->SetDataType(DATA_ACTIVE_POWER);
                } break;
                case 2: {
                    m_text->SetDataType(DATA_REACTIVE_POWER);
                } break;
                case 3: {
                    m_text->SetDataType(DATA_SC_CURRENT);
                } break;
            }
        } break;
        case TYPE_LINE:
        case TYPE_TRANSFORMER: {
            switch(m_choiceTextType->GetSelection()) {
                case 0: {
                    m_text->SetDataType(DATA_NAME);
                } break;
                case 1: {
                    m_text->SetDataType(DATA_PF_ACTIVE);
                } break;
                case 2: {
                    m_text->SetDataType(DATA_PF_REACTIVE);
                } break;
                case 3: {
                    m_text->SetDataType(DATA_PF_LOSSES);
                } break;
                case 4: {
                    m_text->SetDataType(DATA_PF_CURRENT);
                } break;
                case 5: {
                    m_text->SetDataType(DATA_SC_CURRENT);
                } break;
            }
        } break;
        case TYPE_LOAD:
        case TYPE_SYNC_MOTOR:
        case TYPE_IND_MOTOR: {
            switch(m_choiceTextType->GetSelection()) {
                case 0: {
                    m_text->SetDataType(DATA_NAME);
                } break;
                case 1: {
                    m_text->SetDataType(DATA_ACTIVE_POWER);
                } break;
                case 2: {
                    m_text->SetDataType(DATA_REACTIVE_POWER);
                } break;
            }
        } break;
        case TYPE_CAPACITOR:
        case TYPE_INDUCTOR: {
            switch(m_choiceTextType->GetSelection()) {
                case 0: {
                    m_text->SetDataType(DATA_NAME);
                } break;
                case 1: {
                    m_text->SetDataType(DATA_REACTIVE_POWER);
                } break;
            }
        } break;
        default:
            break;
    }
    DataTypeChoice();

    if(m_text->GetDataType() == DATA_NAME) Preview();
}

bool TextForm::LoadChoices()
{
    if(m_text->GetElementType() == TYPE_NONE) return false;

    // Fill the element possible choices.
    ElementTypeChoice();
    m_choiceName->SetSelection(m_text->GetElementNumber());
    ElementNumberChoice();
    DataTypeChoice();

    // Select the saved choices.
    switch(m_text->GetElementType()) {
        case TYPE_BUS: {
            m_choiceElement->SetSelection(0);
            switch(m_text->GetDataType()) {
                case DATA_NAME: {
                    m_choiceTextType->SetSelection(0);
                } break;
                case DATA_VOLTAGE: {
                    m_choiceTextType->SetSelection(1);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_V: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kV: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        default:
                            break;
                    }

                } break;
                case DATA_ANGLE: {
                    m_choiceTextType->SetSelection(2);
                    switch(m_text->GetUnit()) {
                        case UNIT_DEGREE: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_RADIAN: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        default:
                            break;
                    }
                } break;
                case DATA_SC_CURRENT: {
                    m_choiceTextType->SetSelection(3);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_A: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kA: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        default:
                            break;
                    }
                } break;
                case DATA_SC_VOLTAGE: {
                    m_choiceTextType->SetSelection(4);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_V: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kV: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        default:
                            break;
                    }
                } break;
                case DATA_SC_POWER: {
                    m_choiceTextType->SetSelection(5);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_VA: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kVA: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        case UNIT_MVA: {
                            m_choiceTextUnit->SetSelection(3);
                        } break;
                        default:
                            break;
                    }
                } break;
                default:
                    break;
            }
        } break;
        case TYPE_SYNC_GENERATOR: {
            m_choiceElement->SetSelection(1);
            switch(m_text->GetDataType()) {
                case DATA_NAME: {
                    m_choiceTextType->SetSelection(0);
                } break;
                case DATA_ACTIVE_POWER: {
                    m_choiceTextType->SetSelection(1);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_W: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kW: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        case UNIT_MW: {
                            m_choiceTextUnit->SetSelection(3);
                        } break;
                        default:
                            break;
                    }
                } break;
                case DATA_REACTIVE_POWER: {
                    m_choiceTextType->SetSelection(2);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_VAr: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kVAr: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        case UNIT_MVAr: {
                            m_choiceTextUnit->SetSelection(3);
                        } break;
                        default:
                            break;
                    }
                } break;
                case DATA_SC_CURRENT: {
                    m_choiceTextType->SetSelection(3);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_A: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kA: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        default:
                            break;
                    }
                } break;
                default:
                    break;
            }
        } break;
        case TYPE_LINE: {
            m_choiceElement->SetSelection(2);
            switch(m_text->GetDataType()) {
                case DATA_NAME: {
                    m_choiceTextType->SetSelection(0);
                } break;
                case DATA_PF_ACTIVE: {
                    m_choiceTextType->SetSelection(1);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_W: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kW: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        case UNIT_MW: {
                            m_choiceTextUnit->SetSelection(3);
                        } break;
                        default:
                            break;
                    }
                } break;
                case DATA_PF_REACTIVE: {
                    m_choiceTextType->SetSelection(2);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_VAr: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kVAr: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        case UNIT_MVAr: {
                            m_choiceTextUnit->SetSelection(3);
                        } break;
                        default:
                            break;
                    }
                } break;
                case DATA_PF_LOSSES: {
                    m_choiceTextType->SetSelection(3);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_W: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kW: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        case UNIT_MW: {
                            m_choiceTextUnit->SetSelection(3);
                        } break;
                        default:
                            break;
                    }
                } break;
                case DATA_PF_CURRENT: {
                    m_choiceTextType->SetSelection(4);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_A: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kA: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        default:
                            break;
                    }
                } break;
                case DATA_SC_CURRENT: {
                    m_choiceTextType->SetSelection(5);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_A: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kA: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        case UNIT_MW: {
                            m_choiceTextUnit->SetSelection(3);
                        } break;
                        default:
                            break;
                    }
                } break;
                default:
                    break;
            }
        } break;
        case TYPE_TRANSFORMER: {
            m_choiceElement->SetSelection(3);
            switch(m_text->GetDataType()) {
                case DATA_NAME: {
                    m_choiceTextType->SetSelection(0);
                } break;
                case DATA_PF_ACTIVE: {
                    m_choiceTextType->SetSelection(1);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_W: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kW: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        case UNIT_MW: {
                            m_choiceTextUnit->SetSelection(3);
                        } break;
                        default:
                            break;
                    }
                } break;
                case DATA_PF_REACTIVE: {
                    m_choiceTextType->SetSelection(2);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_VAr: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kVAr: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        case UNIT_MVAr: {
                            m_choiceTextUnit->SetSelection(3);
                        } break;
                        default:
                            break;
                    }
                } break;
                case DATA_PF_LOSSES: {
                    m_choiceTextType->SetSelection(3);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_W: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kW: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        case UNIT_MW: {
                            m_choiceTextUnit->SetSelection(3);
                        } break;
                        default:
                            break;
                    }
                } break;
                case DATA_PF_CURRENT: {
                    m_choiceTextType->SetSelection(4);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_A: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kA: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        default:
                            break;
                    }
                } break;
                case DATA_SC_CURRENT: {
                    m_choiceTextType->SetSelection(5);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_A: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kA: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        default:
                            break;
                    }
                } break;
                default:
                    break;
            }
        } break;
        case TYPE_LOAD: {
            m_choiceElement->SetSelection(4);
            switch(m_text->GetDataType()) {
                case DATA_NAME: {
                    m_choiceTextType->SetSelection(0);
                } break;
                case DATA_ACTIVE_POWER: {
                    m_choiceTextType->SetSelection(1);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_W: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kW: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        case UNIT_MW: {
                            m_choiceTextUnit->SetSelection(3);
                        } break;
                        default:
                            break;
                    }
                } break;
                case DATA_REACTIVE_POWER: {
                    m_choiceTextType->SetSelection(2);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_VAr: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kVAr: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        case UNIT_MVAr: {
                            m_choiceTextUnit->SetSelection(3);
                        } break;
                        default:
                            break;
                    }
                } break;
                default:
                    break;
            }
        } break;
        case TYPE_CAPACITOR: {
            m_choiceElement->SetSelection(5);
            switch(m_text->GetDataType()) {
                case DATA_NAME: {
                    m_choiceTextType->SetSelection(0);
                } break;
                case DATA_REACTIVE_POWER: {
                    m_choiceTextType->SetSelection(1);

                } break;
                default:
                    break;
            }
        } break;
        case TYPE_INDUCTOR: {
            m_choiceElement->SetSelection(6);
            switch(m_text->GetDataType()) {
                case DATA_NAME: {
                    m_choiceTextType->SetSelection(0);
                } break;
                case DATA_REACTIVE_POWER: {
                    m_choiceTextType->SetSelection(1);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_VAr: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kVAr: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        case UNIT_MVAr: {
                            m_choiceTextUnit->SetSelection(3);
                        } break;
                        default:
                            break;
                    }
                } break;
                default:
                    break;
            }
        } break;
        case TYPE_SYNC_MOTOR: {
            m_choiceElement->SetSelection(7);
            switch(m_text->GetDataType()) {
                case DATA_NAME: {
                    m_choiceTextType->SetSelection(0);
                } break;
                case DATA_ACTIVE_POWER: {
                    m_choiceTextType->SetSelection(1);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_W: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kW: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        case UNIT_MW: {
                            m_choiceTextUnit->SetSelection(3);
                        } break;
                        default:
                            break;
                    }
                } break;
                case DATA_REACTIVE_POWER: {
                    m_choiceTextType->SetSelection(2);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_VAr: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kVAr: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        case UNIT_MVAr: {
                            m_choiceTextUnit->SetSelection(3);
                        } break;
                        default:
                            break;
                    }
                } break;
                default:
                    break;
            }
        } break;
        case TYPE_IND_MOTOR: {
            m_choiceElement->SetSelection(8);
            switch(m_text->GetDataType()) {
                case DATA_NAME: {
                    m_choiceTextType->SetSelection(0);
                } break;
                case DATA_ACTIVE_POWER: {
                    m_choiceTextType->SetSelection(1);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_W: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kW: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        case UNIT_MW: {
                            m_choiceTextUnit->SetSelection(3);
                        } break;
                        default:
                            break;
                    }
                } break;
                case DATA_REACTIVE_POWER: {
                    m_choiceTextType->SetSelection(2);
                    switch(m_text->GetUnit()) {
                        case UNIT_PU: {
                            m_choiceTextUnit->SetSelection(0);
                        } break;
                        case UNIT_VAr: {
                            m_choiceTextUnit->SetSelection(1);
                        } break;
                        case UNIT_kVAr: {
                            m_choiceTextUnit->SetSelection(2);
                        } break;
                        case UNIT_MVAr: {
                            m_choiceTextUnit->SetSelection(3);
                        } break;
                        default:
                            break;
                    }
                } break;
                default:
                    break;
            }
        } break;
        default:
            break;
    }

    if(m_choiceTextFromBus->IsEnabled()) m_choiceTextFromBus->SetSelection(m_text->GetDirection());
    if(m_choiceTextToBus->IsEnabled()) m_choiceTextToBus->SetSelection(m_text->GetDirection());

    m_textCtrlDecimal->SetValue(wxString::Format("%d", m_text->GetDecimalPlaces()));
    Preview();

    return true;
}

void TextForm::ElementTypeChoice()
{
    m_choiceTextType->Enable(false);
    m_choiceTextFromBus->Enable(false);
    m_choiceTextToBus->Enable(false);
    m_choiceTextUnit->Enable(false);
    m_choiceTextType->Clear();
    m_choiceTextFromBus->Clear();
    m_choiceTextToBus->Clear();
    m_choiceTextUnit->Clear();

    m_choiceName->Clear();
    wxArrayString arrayString;
    switch(m_text->GetElementType()) {
        case TYPE_BUS: {
            for(int i = 0; i < (int)m_allElements.GetBusList().size(); i++) {
                Bus* bus = m_allElements.GetBusList()[i];
                arrayString.Add(bus->GetEletricalData().name);
            }
        } break;
        case TYPE_SYNC_GENERATOR: {
            for(int i = 0; i < (int)m_allElements.GetSyncGeneratorList().size(); i++) {
                SyncGenerator* syncGenerator = m_allElements.GetSyncGeneratorList()[i];
                arrayString.Add(syncGenerator->GetElectricalData().name);
            }
        } break;
        case TYPE_LINE: {
            for(int i = 0; i < (int)m_allElements.GetLineList().size(); i++) {
                Line* line = m_allElements.GetLineList()[i];
                arrayString.Add(line->GetElectricalData().name);
            }
        } break;
        case TYPE_TRANSFORMER: {
            for(int i = 0; i < (int)m_allElements.GetTransformerList().size(); i++) {
                Transformer* transformer = m_allElements.GetTransformerList()[i];
                arrayString.Add(transformer->GetElectricalData().name);
            }
        } break;
        case TYPE_LOAD: {
            for(int i = 0; i < (int)m_allElements.GetLoadList().size(); i++) {
                Load* load = m_allElements.GetLoadList()[i];
                arrayString.Add(load->GetElectricalData().name);
            }
        } break;
        case TYPE_CAPACITOR: {
            for(int i = 0; i < (int)m_allElements.GetCapacitorList().size(); i++) {
                Capacitor* capacitor = m_allElements.GetCapacitorList()[i];
                arrayString.Add(capacitor->GetElectricalData().name);
            }
        } break;
        case TYPE_INDUCTOR: {
            for(int i = 0; i < (int)m_allElements.GetInductorList().size(); i++) {
                Inductor* inductor = m_allElements.GetInductorList()[i];
                arrayString.Add(inductor->GetElectricalData().name);
            }
        } break;
        case TYPE_SYNC_MOTOR: {
            for(int i = 0; i < (int)m_allElements.GetSyncMotorList().size(); i++) {
                SyncMotor* syncMotor = m_allElements.GetSyncMotorList()[i];
                arrayString.Add(syncMotor->GetElectricalData().name);
            }
        } break;
        case TYPE_IND_MOTOR: {
            for(int i = 0; i < (int)m_allElements.GetIndMotorList().size(); i++) {
                IndMotor* indMotor = m_allElements.GetIndMotorList()[i];
                arrayString.Add(indMotor->GetElectricalData().name);
            }
        } break;

        default:
            break;
    }
    m_choiceName->Append(arrayString);
    m_choiceName->Enable();
}

void TextForm::ElementNumberChoice()
{
    m_choiceTextFromBus->Enable(false);
    m_choiceTextToBus->Enable(false);
    m_choiceTextUnit->Enable(false);
    m_choiceTextFromBus->Clear();
    m_choiceTextToBus->Clear();
    m_choiceTextUnit->Clear();

    int index = m_choiceName->GetSelection();
    m_text->SetElementNumber(index);

    m_choiceTextType->Clear();
    wxArrayString arrayString;
    switch(m_text->GetElementType()) {
        case TYPE_BUS: {
            auto it = m_allElements.GetBusList().begin();
            std::advance(it, index);
            m_text->SetElement(*it);

            arrayString.Add(_("Name"));
            arrayString.Add(_("Voltage"));
            arrayString.Add(_("Angle"));
            arrayString.Add(_("Fault current"));
            arrayString.Add(_("Fault voltage"));
            arrayString.Add(_("Short-circuit power"));
        } break;
        case TYPE_SYNC_GENERATOR: {
            auto it = m_allElements.GetSyncGeneratorList().begin();
            std::advance(it, index);
            m_text->SetElement(*it);

            arrayString.Add(_("Name"));
            arrayString.Add(_("Active power"));
            arrayString.Add(_("Reactive power"));
            arrayString.Add(_("Fault current"));
        } break;
        case TYPE_LINE: {
            auto it = m_allElements.GetLineList().begin();
            std::advance(it, index);
            m_text->SetElement(*it);

            arrayString.Add(_("Name"));
            arrayString.Add(_("Active power flow"));
            arrayString.Add(_("Reactive power flow"));
            arrayString.Add(_("Losses"));
            arrayString.Add(_("Current"));
            arrayString.Add(_("Fault current"));
        } break;
        case TYPE_TRANSFORMER: {
            auto it = m_allElements.GetTransformerList().begin();
            std::advance(it, index);
            m_text->SetElement(*it);

            arrayString.Add(_("Name"));
            arrayString.Add(_("Active power flow"));
            arrayString.Add(_("Reactive power flow"));
            arrayString.Add(_("Losses"));
            arrayString.Add(_("Current"));
            arrayString.Add(_("Fault current"));
        } break;
        case TYPE_LOAD: {
            auto it = m_allElements.GetLoadList().begin();
            std::advance(it, index);
            m_text->SetElement(*it);

            arrayString.Add(_("Name"));
            arrayString.Add(_("Active power"));
            arrayString.Add(_("Reactive power"));
        } break;
        case TYPE_CAPACITOR: {
            auto it = m_allElements.GetCapacitorList().begin();
            std::advance(it, index);
            m_text->SetElement(*it);

            arrayString.Add(_("Name"));
            arrayString.Add(_("Reactive power"));
        } break;
        case TYPE_INDUCTOR: {
            auto it = m_allElements.GetInductorList().begin();
            std::advance(it, index);
            m_text->SetElement(*it);

            arrayString.Add(_("Name"));
            arrayString.Add(_("Reactive power"));
        } break;
        case TYPE_SYNC_MOTOR: {
            auto it = m_allElements.GetSyncMotorList().begin();
            std::advance(it, index);
            m_text->SetElement(*it);

            arrayString.Add(_("Name"));
            arrayString.Add(_("Active power"));
            arrayString.Add(_("Reactive power"));
        } break;
        case TYPE_IND_MOTOR: {
            auto it = m_allElements.GetIndMotorList().begin();
            std::advance(it, index);
            m_text->SetElement(*it);

            arrayString.Add(_("Name"));
            arrayString.Add(_("Active power"));
            arrayString.Add(_("Reactive power"));
        } break;

        default:
            break;
    }
    m_choiceTextType->Append(arrayString);
    m_choiceTextType->Enable();
}

void TextForm::DataTypeChoice()
{
    m_choiceTextFromBus->Enable(false);
    m_choiceTextToBus->Enable(false);

    m_choiceTextToBus->Clear();
    m_choiceTextFromBus->Clear();
    m_choiceTextUnit->Clear();

    m_choiceTextUnit->Enable();

    wxArrayString arrayString;
    switch(m_text->GetDataType()) {
        case DATA_NAME: {
            m_choiceTextUnit->Enable(false);
            return;
        } break;
        case DATA_VOLTAGE:
        case DATA_SC_VOLTAGE: {
            arrayString.Add(_("p.u."));
            arrayString.Add(_("V"));
            arrayString.Add(_("kV"));
        } break;
        case DATA_ANGLE: {
            arrayString.Add(_("Degrees"));
            arrayString.Add(_("Radians"));
        } break;
        case DATA_SC_CURRENT:
        case DATA_PF_CURRENT: {
            arrayString.Add(_("p.u."));
            arrayString.Add(_("A"));
            arrayString.Add(_("kA"));
        } break;
        case DATA_SC_POWER: {
            arrayString.Add(_("p.u."));
            arrayString.Add(_("VA"));
            arrayString.Add(_("kVA"));
            arrayString.Add(_("MVA"));
        } break;
        case DATA_ACTIVE_POWER:
        case DATA_PF_ACTIVE:
        case DATA_PF_LOSSES: {
            arrayString.Add(_("p.u."));
            arrayString.Add(_("W"));
            arrayString.Add(_("kW"));
            arrayString.Add(_("MW"));
            m_choiceTextUnit->Enable();
        } break;
        case DATA_REACTIVE_POWER:
        case DATA_PF_REACTIVE: {
            arrayString.Add(_("p.u."));
            arrayString.Add(_("VAr"));
            arrayString.Add(_("kVAr"));
            arrayString.Add(_("MVAr"));
        } break;
        default:
            break;
    }
    m_choiceTextUnit->Append(arrayString);

    switch(m_text->GetElementType()) {
        case TYPE_LINE: {
            if(m_text->GetDataType() != DATA_PF_LOSSES) {
                auto it = m_allElements.GetLineList().begin();
                std::advance(it, m_text->GetElementNumber());
                Line* line = *it;

                Bus* bus1 = (Bus*)line->GetParentList()[0];
                Bus* bus2 = (Bus*)line->GetParentList()[1];
                wxString bus1Name = bus1->GetEletricalData().name;
                wxString bus2Name = bus2->GetEletricalData().name;

                m_choiceTextFromBus->Append(bus1Name);
                m_choiceTextFromBus->Append(bus2Name);
                m_choiceTextToBus->Append(bus2Name);
                m_choiceTextToBus->Append(bus1Name);
                m_choiceTextFromBus->SetSelection(0);
                m_choiceTextToBus->SetSelection(0);

                m_choiceTextFromBus->Enable();
                m_choiceTextToBus->Enable();
            }
        } break;
        case TYPE_TRANSFORMER: {
            if(m_text->GetDataType() != DATA_PF_LOSSES) {
                auto it = m_allElements.GetTransformerList().begin();
                std::advance(it, m_text->GetElementNumber());
                Transformer* transformer = *it;

                Bus* bus1 = (Bus*)transformer->GetParentList()[0];
                Bus* bus2 = (Bus*)transformer->GetParentList()[1];
                wxString bus1Name = bus1->GetEletricalData().name;
                wxString bus2Name = bus2->GetEletricalData().name;

                m_choiceTextFromBus->Append(bus1Name);
                m_choiceTextFromBus->Append(bus2Name);
                m_choiceTextToBus->Append(bus2Name);
                m_choiceTextToBus->Append(bus1Name);
                m_choiceTextFromBus->SetSelection(0);
                m_choiceTextToBus->SetSelection(0);

                m_choiceTextFromBus->Enable();
                m_choiceTextToBus->Enable();
            }
        } break;
        default:
            break;
    }
}

void TextForm::UnitChoice()
{
    switch(m_text->GetDataType()) {
        case DATA_NAME: {
            m_choiceTextUnit->Enable(false);
            return;
        } break;
        case DATA_VOLTAGE:
        case DATA_SC_VOLTAGE: {
            switch(m_choiceTextUnit->GetSelection()) {
                case 0: {
                    m_text->SetUnit(UNIT_PU);
                } break;
                case 1: {
                    m_text->SetUnit(UNIT_V);
                } break;
                case 2: {
                    m_text->SetUnit(UNIT_kV);
                } break;
                default:
                    break;
            }
        } break;
        case DATA_ANGLE: {
            switch(m_choiceTextUnit->GetSelection()) {
                case 0: {
                    m_text->SetUnit(UNIT_DEGREE);
                } break;
                case 1: {
                    m_text->SetUnit(UNIT_RADIAN);
                } break;
                default:
                    break;
            }
        } break;
        case DATA_SC_CURRENT:
        case DATA_PF_CURRENT: {
            switch(m_choiceTextUnit->GetSelection()) {
                case 0: {
                    m_text->SetUnit(UNIT_PU);
                } break;
                case 1: {
                    m_text->SetUnit(UNIT_A);
                } break;
                case 2: {
                    m_text->SetUnit(UNIT_kA);
                } break;
                default:
                    break;
            }
        } break;
        case DATA_SC_POWER: {
            switch(m_choiceTextUnit->GetSelection()) {
                case 0: {
                    m_text->SetUnit(UNIT_PU);
                } break;
                case 1: {
                    m_text->SetUnit(UNIT_VA);
                } break;
                case 2: {
                    m_text->SetUnit(UNIT_kVA);
                } break;
                case 3: {
                    m_text->SetUnit(UNIT_MVA);
                } break;
                default:
                    break;
            }
        } break;
        case DATA_ACTIVE_POWER:
        case DATA_PF_ACTIVE:
        case DATA_PF_LOSSES: {
            switch(m_choiceTextUnit->GetSelection()) {
                case 0: {
                    m_text->SetUnit(UNIT_PU);
                } break;
                case 1: {
                    m_text->SetUnit(UNIT_W);
                } break;
                case 2: {
                    m_text->SetUnit(UNIT_kW);
                } break;
                case 3: {
                    m_text->SetUnit(UNIT_MW);
                } break;
                default:
                    break;
            }
        } break;
        case DATA_REACTIVE_POWER:
        case DATA_PF_REACTIVE: {
            switch(m_choiceTextUnit->GetSelection()) {
                case 0: {
                    m_text->SetUnit(UNIT_PU);
                } break;
                case 1: {
                    m_text->SetUnit(UNIT_VAr);
                } break;
                case 2: {
                    m_text->SetUnit(UNIT_kVAr);
                } break;
                case 3: {
                    m_text->SetUnit(UNIT_MVAr);
                } break;
                default:
                    break;
            }
        } break;
        default:
            break;
    }
}

void TextForm::Preview()
{
    double decimalPlaces = m_text->GetDecimalPlaces();
    if(m_textCtrlDecimal->GetValue().ToDouble(&decimalPlaces)) m_text->SetDecimalPlaces(decimalPlaces);

    m_text->UpdateText(m_systemPowerBase);

    m_textCtrlPreview->SetValue(m_text->GetText());
}

bool TextForm::ValidateData()
{
    if(m_choiceElement->GetSelection() == -1) return false;
    if(m_choiceName->GetSelection() == -1) return false;
    if(m_choiceTextType->GetSelection() == -1) return false;
    if(m_text->GetDataType() != DATA_NAME && m_choiceTextUnit->GetSelection() == -1) return false;
    if(m_text->GetElementType() == TYPE_LINE || m_text->GetElementType() == TYPE_TRANSFORMER) {
        if(m_text->GetDataType() != DATA_PF_LOSSES) {
            if(m_choiceTextFromBus->GetSelection() == -1) return false;
            if(m_choiceTextToBus->GetSelection() == -1) return false;
        }
    }

    if(m_choiceTextFromBus->IsEnabled() && m_choiceTextToBus->IsEnabled())
        m_text->SetDirection(m_choiceTextFromBus->GetSelection());
    double decimalPlaces = m_text->GetDecimalPlaces();
    if(m_textCtrlDecimal->GetValue().ToDouble(&decimalPlaces)) m_text->SetDecimalPlaces(decimalPlaces);

    m_textToEdit->SetElementType(m_text->GetElementType());
    m_textToEdit->SetElementNumber(m_text->GetElementNumber());
    m_textToEdit->SetElement(m_text->GetElement());
    m_textToEdit->SetDataType(m_text->GetDataType());
    m_textToEdit->SetDirection(m_text->GetDirection());
    m_textToEdit->SetUnit(m_text->GetUnit());
    m_textToEdit->SetDecimalPlaces(decimalPlaces);
    m_textToEdit->UpdateText(m_systemPowerBase);

    return true;
}

void TextForm::OnOKButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) {
        EndModal(wxID_OK);
    } else {
        wxString errorMsg = _("There are blank fields.");
        wxMessageDialog msgDialog(this, errorMsg, _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
    }
}
