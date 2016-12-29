#ifndef INDMOTOR_H
#define INDMOTOR_H

#include "Machines.h"

class IndMotorForm;

struct IndMotorElectricalData
{
    wxString name;
    double activePower = 100.0;
    ElectricalUnit activePowerUnit = UNIT_MW;
    double reactivePower = 0.0;
    ElectricalUnit reactivePowerUnit = UNIT_MVAr;
};

class IndMotor : public Machines
{
   public:
    IndMotor();
    IndMotor(wxString name);
    ~IndMotor();
	
	virtual Element* GetCopy();
    virtual void DrawSymbol() const;
    virtual bool GetContextMenu(wxMenu& menu);
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual IndMotorElectricalData GetElectricalData() { return m_electricalData; }
    virtual IndMotorElectricalData GetPUElectricalData(double systemPowerBase);
    virtual void SetElectricalData(IndMotorElectricalData electricalData) { m_electricalData = electricalData; }
   protected:
    IndMotorElectricalData m_electricalData;
};

#endif  // INDMOTOR_H
