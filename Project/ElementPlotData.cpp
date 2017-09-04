#include "ElementPlotData.h"

ElementPlotData::ElementPlotData(wxString name, CurveType curveType)
{
    m_name = name;
    m_curveType = curveType;
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
    data->SetThick(2);

    m_elementData.push_back(data);
}
