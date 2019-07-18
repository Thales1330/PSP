#include "Bus.h"
#include "FrequencyResponseForm.h"

FrequencyResponseForm::FrequencyResponseForm(wxWindow* parent) : FrequencyResponseFormBase(parent)
{
    m_parent = parent;
}

FrequencyResponseForm::FrequencyResponseForm(wxWindow* parent,
                                             std::vector<Bus*> busList,
                                             int injCurrentBus,
                                             double initFreq,
                                             double endFreq,
                                             double stepFreq)
    : FrequencyResponseFormBase(parent)
{
    m_parent = parent;
    m_busList = busList;

    // Set buses numbers and fill the choicebox
    int busNumber = 0;
    for(auto itb = m_busList.begin(); itb != m_busList.end(); itb++) {
        Bus* bus = *itb;
        BusElectricalData data = bus->GetElectricalData();
        data.number = busNumber;
        bus->SetElectricalData(data);

        m_choiceBus->Append(data.name);

        busNumber++;
    }
    
    Bus dummyBus;
    
    m_textCtrlInitFreq->SetValue(dummyBus.StringFromDouble(initFreq));
    m_textCtrlFinalFreq->SetValue(dummyBus.StringFromDouble(endFreq));
    m_textCtrlStepFreq->SetValue(dummyBus.StringFromDouble(stepFreq));
    m_choiceBus->SetSelection(injCurrentBus);
}

FrequencyResponseForm::~FrequencyResponseForm() {}

void FrequencyResponseForm::OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
void FrequencyResponseForm::OnRunButtonClick(wxCommandEvent& event)
{
    Bus dummyBus;
    if(!dummyBus.DoubleFromString(m_parent, m_textCtrlInitFreq->GetValue(), m_initFreq,
                                  _("Value entered incorrectly in the field \"Initial frequency\".")))
        return;
    if(!dummyBus.DoubleFromString(m_parent, m_textCtrlFinalFreq->GetValue(), m_endFreq,
                                  _("Value entered incorrectly in the field \"Final frequency\".")))
        return;
    if(!dummyBus.DoubleFromString(m_parent, m_textCtrlStepFreq->GetValue(), m_stepFreq,
                                  _("Value entered incorrectly in the field \"Frequency step\".")))
        return;
    if(m_choiceBus->GetSelection() == -1) {
        wxMessageDialog msgDialog(m_parent, _("Injected current not selected"), _("Error"),
                                  wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
        return;
    }
    m_injBusNumber = m_choiceBus->GetSelection();
    EndModal(wxID_OK);
}
