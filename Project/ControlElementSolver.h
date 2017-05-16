#ifndef CONTROLELEMENTSOLVER_H
#define CONTROLELEMENTSOLVER_H

#include <wx/window.h>
#include <vector>

class ControlElementContainer;
class ControlEditor;
class ConnectionLine;
class Constant;
class Exponential;
class Gain;
class IOControl;
class Limiter;
class Multiplier;
class RateLimiter;
class Sum;
class TransferFunction;

class ControlElementSolver
{
   public:
    ControlElementSolver() {}
    ControlElementSolver(ControlEditor* controlEditor,
                         double timeStep = 1e-3,
                         double integrationError = 1e-3,
                         bool startAllZero = true,
                         double input = 0.0);
    ControlElementSolver(ControlElementContainer* ctrlContainer,
                         double timeStep = 1e-3,
                         double integrationError = 1e-3,
                         bool startAllZero = true,
                         double input = 0.0,
                         wxWindow* parent = NULL);
    ~ControlElementSolver() {}
    virtual bool InitializeValues(double input, bool startAllZero);
    virtual void SolveNextStep(double input);
    virtual std::vector<double> GetSolutions() { return m_solutions; }
    virtual double GetLastSolution() { return m_solutions[m_solutions.size() - 1]; }
    virtual bool IsOK() const { return m_isOK; }
   protected:
    void Initialize(wxWindow* parent, double timeStep, double integrationError, bool startAllZero, double input);
    void FillAllConnectedChildren(ConnectionLine* parent);
    ConnectionLine* SolveNextElement(ConnectionLine* currentLine);

    ControlElementContainer* m_ctrlContainer = NULL;
    double m_timeStep;
    double m_integrationError;
    std::vector<double> m_solutions;
    bool m_isOK = false;

    IOControl* m_inputControl = NULL;
    IOControl* m_outputControl = NULL;
};

#endif  // CONTROLELEMENTSOLVER_H
