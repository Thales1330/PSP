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

#include "ChartWindowManager.h"
#include "Workspace.h"

ChartWindowManager::ChartWindowManager(Workspace* workspace): m_workspace(workspace)
{
}

ChartWindowManager::~ChartWindowManager()
{
}

ChartView* ChartWindowManager::Open(std::vector<ElementPlotData>& epdList, std::vector<double>& time)
{
	ChartView* cView = new ChartView(m_workspace, epdList, time, m_workspace->GetProperties()->GetGeneralPropertiesData().plotLib);
	cView->Show();
	return cView;
}
