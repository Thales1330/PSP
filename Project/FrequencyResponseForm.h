#ifndef FREQUENCYRESPONSEFORM_H
#define FREQUENCYRESPONSEFORM_H
#include "PropertiesFormBase.h"

class Bus;

class FrequencyResponseForm : public FrequencyResponseFormBase
{
   public:
    FrequencyResponseForm(wxWindow* parent);
    FrequencyResponseForm(wxWindow* parent,
                          std::vector<Bus*> busList,
                          int injCurrentBus,
                          double initFreq,
                          double endFreq,
                          double stepFreq);
    virtual ~FrequencyResponseForm();

    double GetEndFreq() const { return m_endFreq; }
    double GetInitFreq() const { return m_initFreq; }
    double GetStepFreq() const { return m_stepFreq; }
    int GetInjBusNumber() { return m_injBusNumber; }

   protected:
    virtual void OnCancelButtonClick(wxCommandEvent& event);
    virtual void OnRunButtonClick(wxCommandEvent& event);

    std::vector<Bus*> m_busList;
    wxWindow* m_parent;
    int m_injBusNumber = 0;

    double m_initFreq = 0.0;
    double m_endFreq = 1500.0;
    double m_stepFreq = 1.0;
};
#endif  // FREQUENCYRESPONSEFORM_H
