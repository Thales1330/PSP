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
    double nominalVoltage = 138.0;
    ElectricalUnit nominalVoltageUnit = UNIT_kV;
    double nominalPower = 100.0;
    ElectricalUnit nominalPowerUnit = UNIT_MVA;
    double resistance = 0.0;
    ElectricalUnit resistanceUnit = UNIT_PU;
    double indReactance = 1.0;
    ElectricalUnit indReactanceUnit = UNIT_PU;
    double capSusceptance = 0.0;
    ElectricalUnit capSusceptanceUnit = UNIT_PU;
    double lineSize = 100.0;
    bool useLinePower = false;

    // Fault
    double zeroResistance = 0.0;
    double zeroIndReactance = 1.0;
    double zeroCapSusceptance = 0.0;
};

class Transformer : public Branch
{
public:
   Transformer();
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
};

#endif  // TRANSFORMER_H
