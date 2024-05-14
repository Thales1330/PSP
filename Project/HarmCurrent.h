#ifndef HARMCURRENT_H
#define HARMCURRENT_H

#include "Shunt.h"

class HarmCurrentForm;

struct HarmCurrentElectricalData {
    wxString name;
    std::vector<int> harmonicOrder;
    std::vector<double> injHarmCurrent;
    std::vector<ElectricalUnit> injHarmCurrentUnit;
    std::vector<double> injHarmAngle;
};

/**
 * @class HarmCurrent
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 22/04/2019
 * @brief Shunt Harmonic Corrent Source.
 * @file HarmCurrent.h
 */
class HarmCurrent : public Shunt
{
   public:
    HarmCurrent();
    HarmCurrent(wxString name);
    ~HarmCurrent();

    virtual Element* GetCopy();
    virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual void DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const;
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void Rotate(bool clockwise = true);
    virtual bool GetContextMenu(wxMenu& menu);
    virtual wxString GetTipText() const;
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual HarmCurrentElectricalData GetElectricalData() { return m_electricalData; }
    virtual HarmCurrentElectricalData GetPUElectricalData(double systemPowerBase, double voltage);
    virtual void SetElectricalData(HarmCurrentElectricalData electricalData) { m_electricalData = electricalData; }

    virtual rapidxml::xml_node<>* SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode);
    virtual bool OpenElement(rapidxml::xml_node<>* elementNode, std::vector<Element*> parentList);

   protected:
    std::vector<wxPoint2DDouble> m_triangPts;
    HarmCurrentElectricalData m_electricalData;
};

#endif  // HARMCURRENT_H
