#ifndef LOAD_H
#define LOAD_H

#include "LoadForm.h"
#include "Shunt.h"

enum LoadType
{
    CONST_POWER = 0,
    CONST_IMPEDANCE
};

struct LoadElectricalData
{
    wxString name;
    double activePower = 100.0;
    ElectricalUnit activePowerUnit = UNIT_MW;
    double reactivePower = 0.0;
    ElectricalUnit reactivePowerUnit = UNIT_MVAr;
    LoadType loadType = CONST_POWER;
};

class Load : public Shunt
{
   public:
    Load();
    Load(wxString name);
    ~Load();

    virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual void Rotate(bool clockwise = true);
    virtual bool GetContextMenu(wxMenu& menu);
    virtual bool ShowForm(wxWindow* parent, Element* element);
    LoadElectricalData GetElectricalData() { return m_electricalData; }
    void SetElectricalData(LoadElectricalData electricalData) { m_electricalData = electricalData; }

   private:
    std::vector<wxPoint2DDouble> m_triangPts;
    LoadElectricalData m_electricalData;
};

#endif  // LOAD_H
