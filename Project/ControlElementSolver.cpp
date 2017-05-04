#include "ControlElementSolver.h"

#include "ControlElementContainer.h"
#include "ControlEditor.h"
#include "ConnectionLine.h"
#include "Constant.h"
#include "Exponential.h"
#include "Gain.h"
#include "IOControl.h"
#include "Limiter.h"
#include "Multiplier.h"
#include "RateLimiter.h"
#include "Sum.h"
#include "TransferFunction.h"

ControlElementSolver::ControlElementSolver(ControlEditor* controlEditor, double timeStep, bool startAllZero, double input)
{
    m_ctrlContainer = new ControlElementContainer();
    m_ctrlContainer->FillContainer(controlEditor);
    
    //Check if the sistem have one input and one output
    bool fail = false;
    wxString failMessage = "";
    auto ioList = m_ctrlContainer->GetIOControlList();
    if(ioList.size() != 2) {
        fail = true;
        failMessage = _("The control system must have one input and one output.");
    }
    bool haveInput, haveOutput;
    haveInput = haveOutput = false;
    for(auto it = ioList.begin(), itEnd = ioList.end(); it != itEnd; ++it) {
        IOControl* io = *it;
        if(io->GetType() == Node::NODE_OUT) {
            m_inputControl = io;
            haveInput = true;
        }
        else if(io->GetType()  == Node::NODE_IN) {
            m_outputControl = io;
            haveOutput = true;
        }
    }
    if(!fail && !haveInput) {
        fail = true;
        failMessage = _("There is no input in the control system.");
    }
    if(!fail && !haveOutput) {
        fail = true;
        failMessage = _("There is no output in the control system.");
    }
    
    if(fail) {
        wxMessageDialog msgDialog(controlEditor, failMessage, _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
        msgDialog.ShowModal();
    }
    
    m_timeStep = timeStep;
    if(!startAllZero) InitializeValues(input);
}

void ControlElementSolver::InitializeValues(double input)
{
    
}

void ControlElementSolver::SolveNextStep(double input)
{
    auto connectionLineList = m_ctrlContainer->GetConnectionLineList();
    for(auto it = connectionLineList.begin(), itEnd = connectionLineList.end(); it != itEnd; ++it) {
        ConnectionLine* cLine = *it;
        cLine->SetSolved(false);
    }
    // Get first node (connected with input)
    ConnectionLine
}