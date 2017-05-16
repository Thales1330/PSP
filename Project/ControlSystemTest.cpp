#include "ControlSystemTest.h"
#include "ControlEditor.h"

ControlSystemTest::ControlSystemTest(ControlEditor* parent,
                                     int* inputType,
                                     double* startTime,
                                     double* slope,
                                     double* timeStep,
                                     double* simTime)
    : ControlSystemTestBase(parent)
{
    m_inputType = inputType;
    m_startTime = startTime;
    m_slope = slope;
    m_timeStep = timeStep;
    m_simTime = simTime;

    m_choiceInput->SetSelection(*m_inputType);
    m_textCtrlStartTime->SetValue(wxString::FromDouble(*m_startTime));
    m_textCtrlSlope->SetValue(wxString::FromDouble(*m_slope));
    m_textCtrlTimeStep->SetValue(wxString::FromDouble(*m_timeStep));
    m_textCtrlSimTime->SetValue(wxString::FromDouble(*m_simTime));
}

ControlSystemTest::~ControlSystemTest() {}
void ControlSystemTest::OnRunButtonClick(wxCommandEvent& event)
{
    int inputType;
    double startTime, slope, timeStep, simTime;

    inputType = m_choiceInput->GetSelection();

    if(!m_textCtrlStartTime->GetValue().ToDouble(&startTime)) {
        wxMessageDialog msgDialog(this, _("Value entered incorrectly in the field \"Start time\"."), _("Error"),
                                  wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
        return;
    }

    if(!m_textCtrlSlope->GetValue().ToDouble(&slope)) {
        wxMessageDialog msgDialog(this, _("Value entered incorrectly in the field \"Slope\"."), _("Error"),
                                  wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
        return;
    }

    if(!m_textCtrlTimeStep->GetValue().ToDouble(&timeStep)) {
        wxMessageDialog msgDialog(this, _("Value entered incorrectly in the field \"Time step\"."), _("Error"),
                                  wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
        return;
    }

    if(!m_textCtrlSimTime->GetValue().ToDouble(&simTime)) {
        wxMessageDialog msgDialog(this, _("Value entered incorrectly in the field \"Simulation time\"."), _("Error"),
                                  wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
        return;
    }

    *m_inputType = inputType;
    *m_startTime = startTime;
    *m_slope = slope;
    *m_timeStep = timeStep;
    *m_simTime = simTime;

    EndModal(wxID_OK);
}
