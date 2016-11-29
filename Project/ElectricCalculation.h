#ifndef ELECTRICCALCULATION_H
#define ELECTRICCALCULATION_H

#include <vector>
#include <complex>

#include "Element.h"
#include "Bus.h"
#include "Capacitor.h"
#include "IndMotor.h"
#include "Inductor.h"
#include "Line.h"
#include "Load.h"
#include "SyncGenerator.h"
#include "SyncMotor.h"
#include "Transformer.h"

enum BusType { BUS_SLACK = 0, BUS_PV, BUS_PQ };

enum ReactiveLimitsType {
    RL_UNLIMITED = 0,     // The bus can generate any ammount of reactive power.
    RL_LIMITED,           // The bus reactive power generation is limited.
    RL_UNLIMITED_SOURCE,  // The bus have at least one source of infinite reative power.
    RL_MAX_REACHED,       // Max limit reached
    RL_MIN_REACHED,       // Min limit reached
    RL_NONE_REACHED       // No limits reached
};

struct ReactiveLimits {
    double maxLimit = 0.0;
    double minLimit = 0.0;
    ReactiveLimitsType maxLimitType = RL_UNLIMITED;
    ReactiveLimitsType minLimitType = RL_UNLIMITED;
    ReactiveLimitsType limitReached = RL_NONE_REACHED;
};

class ElectricCalculation
{
   public:
    ElectricCalculation();
    ~ElectricCalculation();
    virtual void GetElementsFromList(std::vector<Element*> elementList);
    virtual bool GetYBus(std::vector<std::vector<std::complex<double> > >& yBus, double systemPowerBase);
    virtual void UpdateElementsPowerFlow(std::vector<std::complex<double> > voltage,
                                         std::vector<std::complex<double> > power,
                                         std::vector<BusType> busType,
                                         std::vector<ReactiveLimits> reactiveLimit,
                                         double systemPowerBase);

    const std::vector<Bus*> GetBusList() const { return m_busList; }
    const std::vector<Capacitor*> GetCapacitorList() const { return m_capacitorList; }
    const std::vector<IndMotor*> GetIndMotorList() const { return m_indMotorList; }
    const std::vector<Inductor*> GetInductorList() const { return m_inductorList; }
    const std::vector<Line*> GetLineList() const { return m_lineList; }
    const std::vector<Load*> GetLoadList() const { return m_loadList; }
    const std::vector<SyncGenerator*> GetSyncGeneratorList() const { return m_syncGeneratorList; }
    const std::vector<SyncMotor*> GetSyncMotorList() const { return m_syncMotorList; }
    const std::vector<Transformer*> GetTransformerList() const { return m_transformerList; }
    
   protected:
    std::vector<Bus*> m_busList;
    std::vector<Capacitor*> m_capacitorList;
    std::vector<IndMotor*> m_indMotorList;
    std::vector<Inductor*> m_inductorList;
    std::vector<Line*> m_lineList;
    std::vector<Load*> m_loadList;
    std::vector<SyncGenerator*> m_syncGeneratorList;
    std::vector<SyncMotor*> m_syncMotorList;
    std::vector<Transformer*> m_transformerList;
};

#endif  // ELECTRICCALCULATION_H
