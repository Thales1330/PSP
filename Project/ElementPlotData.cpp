/*
 *  Copyright (C) 2017  Thales Lima Oliveira <thales@ufu.br>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
    data->SetThick(1);

    m_elementData.push_back(data);
}
