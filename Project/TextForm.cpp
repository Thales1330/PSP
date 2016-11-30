#include "TextForm.h"

TextForm::TextForm(wxWindow* parent, Text* text, std::vector<Element*> elementList) : TextFormBase(parent)
{
    m_parent = parent;
    m_text = text;
    m_allElements.GetElementsFromList(elementList);

    m_choiceName->Enable(false);
    m_choiceTextType->Enable(false);
    m_choiceTextFromBus->Enable(false);
    m_choiceTextToBus->Enable(false);
    m_choiceTextUnit->Enable(false);
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

void TextForm::OnFromBusChoiceSelected(wxCommandEvent& event) {}
void TextForm::OnNameChoiceSelected(wxCommandEvent& event)
{
    m_text->SetElementNumber(m_choiceName->GetSelection());
    ElementNumberChoice();
}

void TextForm::OnTextEnter(wxCommandEvent& event) {}
void TextForm::OnToBusChoiceSelected(wxCommandEvent& event) {}
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
                    m_text->SetDataType(DATA_PF_ACTIVE);
                } break;
                case 2: {
                    m_text->SetDataType(DATA_PF_REACTIVE);
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
                    m_text->SetDataType(DATA_PF_REACTIVE);
                } break;
            }
        } break;
        default:
            break;
    }
    DataTypeChoice();
}

void TextForm::ElementTypeChoice()
{
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
    m_choiceTextType->Clear();
    wxArrayString arrayString;
    switch(m_text->GetElementType()) {
        case TYPE_BUS: {
            arrayString.Add(_("Name"));
            arrayString.Add(_("Voltage"));
            arrayString.Add(_("Angle"));
            arrayString.Add(_("Fault current"));
            arrayString.Add(_("Fault voltage"));
            arrayString.Add(_("Short-circuit power"));
        } break;
        case TYPE_SYNC_GENERATOR: {
            arrayString.Add(_("Name"));
            arrayString.Add(_("Active power"));
            arrayString.Add(_("Reactive power"));
            arrayString.Add(_("Fault current"));
        } break;
        case TYPE_LINE:
        case TYPE_TRANSFORMER: {
            arrayString.Add(_("Name"));
            arrayString.Add(_("Active power flow"));
            arrayString.Add(_("Reactive power flow"));
            arrayString.Add(_("Losses"));
            arrayString.Add(_("Current"));
            arrayString.Add(_("Fault current"));
        } break;
        case TYPE_LOAD: {
            arrayString.Add(_("Name"));
            arrayString.Add(_("Active power"));
            arrayString.Add(_("Reactive power"));
        } break;
        case TYPE_CAPACITOR:
        case TYPE_INDUCTOR: {
            arrayString.Add(_("Name"));
            arrayString.Add(_("Reactive power"));
        } break;
        case TYPE_SYNC_MOTOR:
        case TYPE_IND_MOTOR: {
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
        } break;
        case TYPE_TRANSFORMER: {
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
        } break;
        default:
            break;
    }
}

void TextForm::FromChoice() {}
void TextForm::ToChoice() {}
void TextForm::UnitChoice() {}
