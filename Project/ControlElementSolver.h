#ifndef CONTROLELEMENTSOLVER_H
#define CONTROLELEMENTSOLVER_H

#include <stddef.h>  // NULL definition
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
                         bool startAllZero = false,
                         double input = 0.0);
    ~ControlElementSolver() {}
    virtual void InitializeValues(double input, bool startAllZero);
    virtual void SolveNextStep(double input);
    virtual std::vector<double> GetSolutions() { return m_solutions; }
    virtual double GetLastSolution() { return m_solutions[m_solutions.size() - 1]; }
   protected:
    void FillAllConnectedChildren(ConnectionLine* parent);
    ConnectionLine* SolveNextElement(ConnectionLine* currentLine);

    ControlElementContainer* m_ctrlContainer = NULL;
    double m_timeStep;
    double m_integrationError;
    std::vector<double> m_solutions;

    IOControl* m_inputControl = NULL;
    IOControl* m_outputControl = NULL;
};

#endif  // CONTROLELEMENTSOLVER_H
