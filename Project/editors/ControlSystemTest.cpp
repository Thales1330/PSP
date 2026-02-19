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

#include "ControlSystemTest.h"
#include "ControlEditor.h"

ControlSystemTest::ControlSystemTest(ControlEditor* parent,
	std::vector<IOControl*> ioList,
	int* inputType,
	double* startTime,
	double* slope,
	double* timeStep,
	double* simTime)
	: ControlSystemTestBase(parent)
{
	SetSize(GetBestSize());

	for (auto* io : ioList) {
		if (io->GetType() == Node::NodeType::NODE_OUT) {
			m_choiceInput->Append(io->GetName(), io);
		}
	}

	m_inputType = inputType;
	//m_startTime = startTime;
	//m_slope = slope;
	m_timeStep = timeStep;
	m_simTime = simTime;

	m_choiceInput->SetSelection(*m_inputType);
	//m_textCtrlStartTime->SetValue(wxString::FromDouble(*m_startTime));
	//m_textCtrlSlope->SetValue(wxString::FromDouble(*m_slope));
	m_textCtrlTimeStep->SetValue(wxString::FromDouble(*m_timeStep));
	m_textCtrlSimTime->SetValue(wxString::FromDouble(*m_simTime));

	UpdatePGValues();
}

ControlSystemTest::~ControlSystemTest() {}
void ControlSystemTest::OnRunButtonClick(wxCommandEvent& event)
{
	int inputType;
	double timeStep, simTime;

	inputType = m_choiceInput->GetSelection();

	//if (!m_textCtrlStartTime->GetValue().ToDouble(&startTime)) {
	//	wxMessageDialog msgDialog(this, _("Value entered incorrectly in the field \"Start time\"."), _("Error"),
	//		wxOK | wxCENTRE | wxICON_ERROR);
	//	msgDialog.ShowModal();
	//	return;
	//}
	//
	//if (!m_textCtrlSlope->GetValue().ToDouble(&slope)) {
	//	wxMessageDialog msgDialog(this, _("Value entered incorrectly in the field \"Slope\"."), _("Error"),
	//		wxOK | wxCENTRE | wxICON_ERROR);
	//	msgDialog.ShowModal();
	//	return;
	//}

	if (!m_textCtrlTimeStep->GetValue().ToDouble(&timeStep)) {
		wxMessageDialog msgDialog(this, _("Value entered incorrectly in the field \"Time step\"."), _("Error"),
			wxOK | wxCENTRE | wxICON_ERROR);
		msgDialog.ShowModal();
		return;
	}

	if (!m_textCtrlSimTime->GetValue().ToDouble(&simTime)) {
		wxMessageDialog msgDialog(this, _("Value entered incorrectly in the field \"Simulation time\"."), _("Error"),
			wxOK | wxCENTRE | wxICON_ERROR);
		msgDialog.ShowModal();
		return;
	}

	*m_inputType = inputType;
	//*m_startTime = startTime;
	//*m_slope = slope;
	*m_timeStep = timeStep;
	*m_simTime = simTime;

	EndModal(wxID_OK);
}

void ControlSystemTest::OnInputSelected(wxCommandEvent& event)
{
	UpdatePGValues();
}
void ControlSystemTest::OnPGValueChange(wxPropertyGridEvent& event)
{
	IOControl* io = static_cast<IOControl*>(m_choiceInput->GetClientData(m_choiceInput->GetSelection()));
	if (io == nullptr) return;

	SimTestData simTestData = io->GetSimTestData();
	if (event.GetProperty() == m_pgPropStartTime) {
		simTestData.startTime = m_pgPropStartTime->GetValue();
	}
	else if (event.GetProperty() == m_pgPropInitialValue) {
		simTestData.initialValue = m_pgPropInitialValue->GetValue();
	}
	else if (event.GetProperty() == m_pgPropSlope) {
		simTestData.slope = m_pgPropSlope->GetValue();
	}
	else if (event.GetProperty() == m_pgPropInputType) {
		simTestData.type = m_pgPropInputType->GetValue().GetInteger();
	}
	io->SetSimTestData(simTestData);
}

void ControlSystemTest::UpdatePGValues()
{
	IOControl* io = static_cast<IOControl*>(m_choiceInput->GetClientData(m_choiceInput->GetSelection()));
	if (io != nullptr) {
		SimTestData simTestData = io->GetSimTestData();
		m_pgMgr->ChangePropertyValue(m_pgPropStartTime, wxString::FromDouble(simTestData.startTime));
		m_pgMgr->ChangePropertyValue(m_pgPropInitialValue, wxString::FromDouble(simTestData.initialValue));
		m_pgMgr->ChangePropertyValue(m_pgPropSlope, wxString::FromDouble(simTestData.slope));
		m_pgMgr->ChangePropertyValue(m_pgPropInputType, simTestData.type);
	}
}
