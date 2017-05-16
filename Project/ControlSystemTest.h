#ifndef CONTROLSYSTEMTEST_H
#define CONTROLSYSTEMTEST_H

#include "ControlEditorBase.h"

class ControlEditor;

class ControlSystemTest : public ControlSystemTestBase
{
public:
    ControlSystemTest(ControlEditor* parent, int* inputType, double* startTime, double* slope, double* timeStep, double* simTime);
    virtual ~ControlSystemTest();
    
protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnRunButtonClick(wxCommandEvent& event);
    int* m_inputType = NULL;
    double* m_startTime = NULL;
    double* m_slope = NULL;
    double* m_timeStep = NULL;
    double* m_simTime = NULL;
};
#endif // CONTROLSYSTEMTEST_H
