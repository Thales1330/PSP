#ifndef CONTROLELEMENTCONTAINER_H
#define CONTROLELEMENTCONTAINER_H

class ControlEditor;

class ControlElement;
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

class ControlElementContainer
{
   public:
    ControlElementContainer();
    ~ControlElementContainer();

    virtual void FillContainer(ControlEditor* editor);
    virtual void ClearContainer();

    std::vector<ConnectionLine*> GetConnectionLineList() const { return m_cLineList; }
    std::vector<Constant*> GetConstantList() const { return m_constantList; }
    std::vector<Exponential*> GetExponentialList() const { return m_exponentialList; }
    std::vector<Gain*> GetGainList() const { return m_gainList; }
    std::vector<IOControl*> GetIOControlList() const { return m_ioControlList; }
    std::vector<Limiter*> GetLimiterList() const { return m_limiterList; }
    std::vector<Multiplier*> GetMultiplierList() const { return m_multiplierList; }
    std::vector<RateLimiter*> GetRateLimiterList() const { return m_rateLimiterList; }
    std::vector<Sum*> GetSumList() const { return m_sumList; }
    std::vector<TransferFunction*> GetTFList() const { return m_tfList; }
   protected:
    std::vector<ConnectionLine*> m_cLineList;
    std::vector<Constant*> m_constantList;
    std::vector<Exponential*> m_exponentialList;
    std::vector<Gain*> m_gainList;
    std::vector<IOControl*> m_ioControlList;
    std::vector<Limiter*> m_limiterList;
    std::vector<Multiplier*> m_multiplierList;
    std::vector<RateLimiter*> m_rateLimiterList;
    std::vector<Sum*> m_sumList;
    std::vector<TransferFunction*> m_tfList;
};

#endif  // CONTROLELEMENTCONTAINER_H
