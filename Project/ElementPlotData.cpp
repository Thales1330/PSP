#include "ElementPlotData.h"

ElementPlotData::ElementPlotData(wxString name, CurveType type)
{
    m_name = name;
    m_curveType = type;
}

ElementPlotData::~ElementPlotData() {}
void ElementPlotData::AddData(std::vector<double> values, wxString name)
{
    PlotData* data = new PlotData();
    data->SetName(name);
    data->SetValues(values);
    data->SetPlot(false);
    data->SetAxis(0);
    data->SetColour(*wxBLACK);
    data->SetPenType(wxPENSTYLE_SOLID);
    data->SetThick(1);

    m_elementData.push_back(data);
}
