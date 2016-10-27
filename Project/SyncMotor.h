#ifndef SYNCMOTOR_H
#define SYNCMOTOR_H

#include "Machines.h"

class SyncMachineForm;

struct SyncMotorElectricalData {
    // General
    wxString name = "";
    double nominalPower = 100.0;
    ElectricalUnit nominalPowerUnit = UNIT_MVA;
    double activePower = 100.0;
    ElectricalUnit activePowerUnit = UNIT_MVA;
    double reactivePower = 0.0;
    ElectricalUnit reactivePowerUnit = UNIT_MVA;
    bool haveMaxReactive = false;
    double maxReactive = 9999.0;
    ElectricalUnit maxReactiveUnit = UNIT_MVA;
    bool haveMinReactive = false;
    double minReactive = -9999.0;
    ElectricalUnit minReactiveUnit = UNIT_MVA;
    bool useMachineBase = false;

    // Fault
    double positiveResistance = 0.0;
    double positiveReactance = 1.0;
    double negativeResistance = 0.0;
    double negativeReactance = 1.0;
    double zeroResistance = 0.0;
    double zeroReactance = 1.0;
    double groundResistance = 0.0;
    double groundReactance = 0.0;
    bool groundNeutral = true;

    // Stability
    bool plotSyncMachine = false;
    double inertia = 0.0;
    double damping = 0.0;
    bool useAVR = false;
    bool useSpeedGovernor = false;

    double armResistance = 0.0;
    double potierReactance = 0.0;
    double satFactor = 0.0;

    double syncXd = 0.0;
    double syncXq = 0.0;
    double transXd = 1.0;
    double transXq = 0.0;
    double transTd0 = 0.0;
    double transTq0 = 0.0;
    double subXd = 0.0;
    double subXq = 0.0;
    double subTd0 = 0.0;
    double subTq0 = 0.0;
};

class SyncMotor : public Machines
{
   public:
    SyncMotor();
    SyncMotor(wxString name);
    ~SyncMotor();

    virtual void DrawSymbol() const;
    virtual bool GetContextMenu(wxMenu& menu);
    virtual SyncMotorElectricalData GetElectricalData() { return m_electricalData; }
    virtual void SetElectricalData(SyncMotorElectricalData electricalData) { m_electricalData = electricalData; }
    virtual bool ShowForm(wxWindow* parent, Element* element);

   protected:
    SyncMotorElectricalData m_electricalData;
};

#endif  // SYNCMOTOR_H
