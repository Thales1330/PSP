#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include "Branch.h"

class TransformerForm;

enum TransformerConnection
{
    GWYE_GWYE = 0,
    WYE_GWYE,
    GWYE_WYE,
    WYE_WYE,
    DELTA_GWYE,
    DELTA_WYE,
    GWYE_DELTA,
    WYE_DELTA,
    DELTA_DELTA
};

struct TransformerElectricalData
{
    // General
    wxString name = "";
    double primaryNominalVoltage = 138.0;
    ElectricalUnit primaryNominalVoltageUnit = UNIT_kV;
    double secondaryNominalVoltage = 138.0;
    ElectricalUnit secondaryNominalVoltageUnit = UNIT_kV;
    int baseVoltage = 0;
    double nominalPower = 100.0;
    ElectricalUnit nominalPowerUnit = UNIT_MVA;
    double resistance = 0.0;
    ElectricalUnit resistanceUnit = UNIT_PU;
    double indReactance = 1.0;
    ElectricalUnit indReactanceUnit = UNIT_PU;
    TransformerConnection connection = GWYE_GWYE;
    double turnsRatio = 1.0;
    double phaseShift = 0.0;
    bool useTransformerPower = false;

    // Fault
    double zeroResistance = 0.0;
    double zeroIndReactance = 1.0;
    double primaryGrndResistance = 0.0;
    double primaryGrndReactance = 0.0;
    double secondaryGrndResistance = 0.0;
    double secondaryGrndReactance = 0.0;
};

class Transformer : public Branch
{
public:
   Transformer();
   Transformer(wxString name);
    virtual ~Transformer();
	
    virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void Rotate(bool clockwise = true);
	virtual void Move(wxPoint2DDouble position);
	virtual void MoveNode(Element* parent, wxPoint2DDouble position);
	virtual void StartMove(wxPoint2DDouble position);
	virtual bool GetContextMenu(wxMenu& menu);
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual TransformerElectricalData GetElectricalData() { return m_electricalData; }
    virtual void SetElectricaData(TransformerElectricalData electricalData) { m_electricalData = electricalData; }
    virtual void SetNominalVoltage(std::vector<double> nominalVoltage, std::vector<ElectricalUnit> nominalVoltageUnit);
    
protected:
    TransformerElectricalData m_electricalData;
};

#endif  // TRANSFORMER_H
