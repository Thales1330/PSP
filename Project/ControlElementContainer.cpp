#include "ControlElementContainer.h"
#include "ControlEditor.h"
#include "ControlElement.h"

ControlElementContainer::ControlElementContainer() { ClearContainer(); }
ControlElementContainer::~ControlElementContainer() {}
void ControlElementContainer::FillContainer(ControlEditor* editor)
{
    ClearContainer();
    m_ctrlElementsList = editor->GetControlElementList();
    m_cLineList = editor->GetConnectionLineList();
    auto cElementList = editor->GetControlElementList();
    for(auto it = cElementList.begin(), itEnd = cElementList.end(); it != itEnd; ++it) {
        if(Constant* constant = dynamic_cast<Constant*>(*it)) {
            m_constantList.push_back(constant);
        } else if(Exponential* exponential = dynamic_cast<Exponential*>(*it)) {
            m_exponentialList.push_back(exponential);
        } else if(Gain* gain = dynamic_cast<Gain*>(*it)) {
            m_gainList.push_back(gain);
        } else if(IOControl* ioControl = dynamic_cast<IOControl*>(*it)) {
            m_ioControlList.push_back(ioControl);
        } else if(Limiter* limiter = dynamic_cast<Limiter*>(*it)) {
            m_limiterList.push_back(limiter);
        } else if(Multiplier* multiplier = dynamic_cast<Multiplier*>(*it)) {
            m_multiplierList.push_back(multiplier);
        } else if(RateLimiter* rateLimiter = dynamic_cast<RateLimiter*>(*it)) {
            m_rateLimiterList.push_back(rateLimiter);
        } else if(Sum* sum = dynamic_cast<Sum*>(*it)) {
            m_sumList.push_back(sum);
        } else if(TransferFunction* tf = dynamic_cast<TransferFunction*>(*it)) {
            m_tfList.push_back(tf);
        }
    }
}

void ControlElementContainer::ClearContainer()
{
    m_cLineList.clear();
    m_constantList.clear();
    m_exponentialList.clear();
    m_gainList.clear();
    m_ioControlList.clear();
    m_limiterList.clear();
    m_multiplierList.clear();
    m_rateLimiterList.clear();
    m_sumList.clear();
    m_tfList.clear();
}

void ControlElementContainer::FillContainer(std::vector<ControlElement*> controlElementList,
                                            std::vector<ConnectionLine*> connectionLineList)
{
    m_ctrlElementsList = controlElementList;
    m_cLineList = connectionLineList;
}
