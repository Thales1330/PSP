#ifndef INDUCTOR_H
#define INDUCTOR_H

#include "Shunt.h"

class ReactiveShuntElementForm;

struct InductorElectricalData {
    wxString name;
    double reactivePower = 100.0;
    ElectricalUnit reactivePowerUnit = UNIT_MVAr;
};

class Inductor : public Shunt
{
   public:
    Inductor();
    Inductor(wxString name);
    ~Inductor();

    virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void Rotate(bool clockwise = true);
    virtual bool GetContextMenu(wxMenu& menu);
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual InductorElectricalData GetElectricalData() { return m_electricalData; }
    virtual InductorElectricalData GetPUElectricalData(double systemPowerBase);
    virtual void SetElectricalData(InductorElectricalData electricalData) { m_electricalData = electricalData; }
   protected:
    InductorElectricalData m_electricalData;
};

#endif  // INDUCTOR_H
