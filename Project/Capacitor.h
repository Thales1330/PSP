#ifndef CAPACITOR_H
#define CAPACITOR_H

#include "Shunt.h"

class ReactiveShuntElementForm;

struct CapacitorElectricalData {
    wxString name;
    double reactivePower = 100.0;
    ElectricalUnit reactivePowerUnit = UNIT_MVAr;
};

class Capacitor : public Shunt
{
   public:
    Capacitor();
    Capacitor(wxString name);
    ~Capacitor();

    virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void Rotate(bool clockwise = true);
    virtual bool GetContextMenu(wxMenu& menu);
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual CapacitorElectricalData GetElectricalData() { return m_electricalData; }
    virtual void SetElectricalData(CapacitorElectricalData electricalData) { m_electricalData = electricalData; }
   protected:
    CapacitorElectricalData m_electricalData;
};

#endif  // CAPACITOR_H
