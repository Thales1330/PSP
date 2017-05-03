#ifndef ELEMENTPLOTDATA_H
#define ELEMENTPLOTDATA_H

#include <wx/treectrl.h>
#include <wx/colour.h>
#include <wx/pen.h>

#include <vector>

class PlotData : public wxTreeItemData
{
   public:
    PlotData() {}
    ~PlotData() {}
    void SetAxis(int axis) { m_axis = axis; }
    void SetColour(const wxColour& colour) { m_colour = colour; }
    void SetName(const wxString& name) { m_name = name; }
    void SetPenType(const wxPenStyle& penType) { m_penType = penType; }
    void SetPlot(bool plot) { m_plot = plot; }
    void SetThick(int thick) { m_thick = thick; }
    void SetValues(const std::vector<double>& values) { m_values = values; }
    int GetAxis() const { return m_axis; }
    wxColour GetColour() const { return m_colour; }
    wxString GetName() const { return m_name; }
    wxPenStyle GetPenType() const { return m_penType; }
    bool IsPlot() const { return m_plot; }
    int GetThick() const { return m_thick; }
    std::vector<double> GetValues() const { return m_values; }
   protected:
    std::vector<double> m_values;
    wxString m_name;
    bool m_plot;
    wxColour m_colour;
    int m_thick;
    wxPenStyle m_penType;
    int m_axis;
};

class ElementPlotData
{
   public:
    enum CurveType {
        CT_BUS = 0,
        CT_SYNC_GENERATOR,
        CT_SYNC_COMPENSATOR,
        CT_TRANSFORMER,
        CT_LINE,
        CT_IND_MOTOR,
        CT_SHUNT_INDUCTOR,
        CT_SHUNT_CAPACITOR,
        CT_LOAD,
        NUM_ELEMENTS,
        CT_TIME
    };
    ElementPlotData() {};
    ElementPlotData(wxString name, CurveType curveType);
    ~ElementPlotData();

    wxString GetName() const { return m_name; }
    void SetName(wxString name) { m_name = name; }
    CurveType GetCurveType() const { return m_curveType; }
    PlotData* GetPlotData(int index) const { return m_elementData[index]; }
    void AddData(std::vector<double> values, wxString name);

    int GetElementDataNumber() const { return static_cast<int>(m_elementData.size()); }
    std::vector<double> GetValues(int index) const { return m_elementData[index]->GetValues(); }
    void SetValues(int index, std::vector<double> values) { m_elementData[index]->SetValues(values); }
    wxString GetDataName(int index) const { return m_elementData[index]->GetName(); }
    void SetDataName(int index, wxString name) { m_elementData[index]->SetName(name); }
    wxColour GetColour(int index) const { return m_elementData[index]->GetColour(); }
    void SetColour(int index, wxColour colour) { m_elementData[index]->SetColour(colour); }
    int GetThick(int index) const { return m_elementData[index]->GetThick(); }
    void SetThick(int index, int thick) { m_elementData[index]->SetThick(thick); }
    wxPenStyle GetPenType(int index) const { return m_elementData[index]->GetPenType(); }
    void SetPenType(int index, wxPenStyle penType) { m_elementData[index]->SetPenType(penType); }
    int GetAxis(int index) const { return m_elementData[index]->GetAxis(); }
    void SetAxis(int index, int axis) { m_elementData[index]->SetAxis(axis); }
   protected:
    wxString m_name;
    CurveType m_curveType;
    std::vector<PlotData*> m_elementData;
};

#endif  // ELEMENTPLOTDATA_H
