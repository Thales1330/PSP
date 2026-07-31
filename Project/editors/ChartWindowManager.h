/*
 *  Copyright (C) 2026  Thales Lima Oliveira <thales.oliveira@uftm.edu.br>
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

#ifndef CHARTWINDOWMANAGER_H
#define CHARTWINDOWMANAGER_H

class Workspace;
#include "ChartView.h"
#include "../utils/ElementPlotData.h"

class ChartWindowManager
{
public:
    ChartWindowManager(Workspace* workspace);
    ~ChartWindowManager();

    ChartView* Open(std::vector<ElementPlotData>& epdList, std::vector<double>& time);

private:
    Workspace* m_workspace = nullptr;
};

#endif

