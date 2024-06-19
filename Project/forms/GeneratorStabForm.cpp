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

#include "GeneratorStabForm.h"

#include "../editors/ControlEditor.h"

#include "SwitchingForm.h"

#include "../elements/controlElement/ControlElementContainer.h"
#include "../elements/powerElement/SyncGenerator.h"

GeneratorStabForm::GeneratorStabForm(wxWindow* parent, SyncGenerator* syncGenerator, int plotLib)
	: GeneratorStabFormBase(parent), m_parent(parent), m_syncGenerator(syncGenerator), m_plotLib(plotLib)
{
	SetSize(GetBestSize());
	//m_syncGenerator = syncGenerator;
	//m_parent = parent;
	//m_sharedGLContext = sharedGLContext;

	SyncGeneratorElectricalData data = syncGenerator->GetElectricalData();

	m_checkBoxPlotSyncMachine->SetValue(data.plotSyncMachine);

	m_textCtrlInertia->SetValue(SyncGenerator::StringFromDouble(data.inertia));
	m_textCtrlDamping->SetValue(SyncGenerator::StringFromDouble(data.damping));

	m_checkBoxUseAVR->SetValue(data.useAVR);
	m_buttonEditAVR->Enable(data.useAVR);

	m_checkBoxUseSG->SetValue(data.useSpeedGovernor);
	m_buttonEditSG->Enable(data.useSpeedGovernor);

	m_textCtrlRa->SetValue(SyncGenerator::StringFromDouble(data.armResistance));
	m_textCtrlXp->SetValue(SyncGenerator::StringFromDouble(data.potierReactance));
	m_textCtrlSat->SetValue(SyncGenerator::StringFromDouble(data.satFactor));
	m_textCtrlOCFreq->SetValue(SyncGenerator::StringFromDouble(data.ocFrequency));

	m_textCtrlSyncXd->SetValue(SyncGenerator::StringFromDouble(data.syncXd));
	m_textCtrlSyncXq->SetValue(SyncGenerator::StringFromDouble(data.syncXq));

	m_textCtrlTranXd->SetValue(SyncGenerator::StringFromDouble(data.transXd));
	m_textCtrlTranXq->SetValue(SyncGenerator::StringFromDouble(data.transXq));
	m_textCtrlTranTd0->SetValue(SyncGenerator::StringFromDouble(data.transTd0));
	m_textCtrlTranTq0->SetValue(SyncGenerator::StringFromDouble(data.transTq0));

	m_textCtrlSubXd->SetValue(SyncGenerator::StringFromDouble(data.subXd));
	m_textCtrlSubXq->SetValue(SyncGenerator::StringFromDouble(data.subXq));
	m_textCtrlSubTd0->SetValue(SyncGenerator::StringFromDouble(data.subTd0));
	m_textCtrlSubTq0->SetValue(SyncGenerator::StringFromDouble(data.subTq0));
}

GeneratorStabForm::~GeneratorStabForm()
{
}
void GeneratorStabForm::OnCancelButtonClick(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}
void GeneratorStabForm::OnEditAVRButtonClick(wxCommandEvent& event)
{
	if (ValidateData()) {
		SyncGeneratorElectricalData data = m_syncGenerator->GetElectricalData();
		if (!data.avr) {
			data.avr = new ControlElementContainer();
			m_syncGenerator->SetElectricalData(data);
		}

		ControlEditor* cEditor = nullptr;

		//if (m_sharedGLContext) {
		//	cEditor = new ControlEditor(nullptr, m_sharedGLContext, IOControl::IN_TERMINAL_VOLTAGE | IOControl::IN_ACTIVE_POWER |
		//		IOControl::IN_REACTIVE_POWER | IOControl::IN_INITIAL_TERMINAL_VOLTAGE | IOControl::IN_VELOCITY |
		//		IOControl::IN_INITIAL_VELOCITY | IOControl::IN_DELTA_VELOCITY | IOControl::IN_DELTA_ACTIVE_POWER |
		//		IOControl::OUT_FIELD_VOLTAGE);
		//}
		//else {
		//	cEditor = new ControlEditorDC(nullptr, IOControl::IN_TERMINAL_VOLTAGE | IOControl::IN_ACTIVE_POWER |
		//		IOControl::IN_REACTIVE_POWER | IOControl::IN_INITIAL_TERMINAL_VOLTAGE | IOControl::IN_VELOCITY |
		//		IOControl::IN_INITIAL_VELOCITY | IOControl::IN_DELTA_VELOCITY | IOControl::IN_DELTA_ACTIVE_POWER |
		//		IOControl::OUT_FIELD_VOLTAGE);
		//}
		cEditor = new ControlEditor(nullptr, IOControl::IN_TERMINAL_VOLTAGE | IOControl::IN_ACTIVE_POWER |
			IOControl::IN_REACTIVE_POWER | IOControl::IN_INITIAL_TERMINAL_VOLTAGE | IOControl::IN_VELOCITY |
			IOControl::IN_INITIAL_VELOCITY | IOControl::IN_DELTA_VELOCITY | IOControl::IN_DELTA_ACTIVE_POWER |
			IOControl::OUT_FIELD_VOLTAGE);

		cEditor->SetElementsList(data.avr->GetControlElementsList());
		cEditor->SetConnectionsList(data.avr->GetConnectionLineList());
		cEditor->SetControlContainer(data.avr);
		cEditor->SetPlotLib(m_plotLib);
		cEditor->Show();
		cEditor->SetJustOpened(true);
#ifdef __WXGTK__
		EndModal(wxID_OK);
#endif
	}
}

void GeneratorStabForm::OnOKButtonClick(wxCommandEvent& event)
{
	if (ValidateData())
		EndModal(wxID_OK);
}

void GeneratorStabForm::OnSpeedGovernorButtonClick(wxCommandEvent& event)
{
	if (ValidateData()) {
		SyncGeneratorElectricalData data = m_syncGenerator->GetElectricalData();
		if (!data.speedGov) {
			data.speedGov = new ControlElementContainer();
			m_syncGenerator->SetElectricalData(data);
		}
		ControlEditor* cEditor = nullptr;

		//if (m_sharedGLContext) {
		//    cEditor = new ControlEditor(nullptr, m_sharedGLContext, IOControl::IN_VELOCITY | IOControl::IN_ACTIVE_POWER | IOControl::IN_REACTIVE_POWER |
		//        IOControl::IN_INITIAL_VELOCITY | IOControl::IN_INITIAL_MEC_POWER | IOControl::OUT_MEC_POWER);
		//}
		//else {
		//	cEditor = new ControlEditorDC(nullptr, IOControl::IN_VELOCITY | IOControl::IN_ACTIVE_POWER | IOControl::IN_REACTIVE_POWER |
		//		IOControl::IN_INITIAL_VELOCITY | IOControl::IN_INITIAL_MEC_POWER | IOControl::OUT_MEC_POWER);
		//}
		cEditor = new ControlEditor(nullptr, IOControl::IN_VELOCITY | IOControl::IN_ACTIVE_POWER | IOControl::IN_REACTIVE_POWER |
			IOControl::IN_INITIAL_VELOCITY | IOControl::IN_INITIAL_MEC_POWER | IOControl::OUT_MEC_POWER);

		cEditor->SetElementsList(data.speedGov->GetControlElementsList());
		cEditor->SetConnectionsList(data.speedGov->GetConnectionLineList());
		cEditor->SetControlContainer(data.speedGov);
		cEditor->SetPlotLib(m_plotLib);
		cEditor->Show();
		cEditor->SetJustOpened(true);
#ifdef __WXGTK__
		EndModal(wxID_OK);
#endif
	}
}

void GeneratorStabForm::OnSwitchingButtonClick(wxCommandEvent& event)
{
	if (ValidateData()) {
		SwitchingForm swForm(m_parent, m_syncGenerator);
		swForm.SetTitle(_("Synchronous generator: Switching"));
		swForm.ShowModal();
		EndModal(wxID_OK);
	}
}

bool GeneratorStabForm::ValidateData()
{
	SyncGeneratorElectricalData data = m_syncGenerator->GetElectricalData();

	data.plotSyncMachine = m_checkBoxPlotSyncMachine->GetValue();

	if (!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlInertia->GetValue(), data.inertia,
		_("Value entered incorrectly in the field \"Inertia\".")))
		return false;

	if (!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlDamping->GetValue(), data.damping,
		_("Value entered incorrectly in the field \"Damping factor\".")))
		return false;

	data.useAVR = m_checkBoxUseAVR->GetValue();
	data.useSpeedGovernor = m_checkBoxUseSG->GetValue();

	if (!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlRa->GetValue(), data.armResistance,
		_("Value entered incorrectly in the field \"Armature resistance\".")))
		return false;

	if (!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlXp->GetValue(), data.potierReactance,
		_("Value entered incorrectly in the field \"Potier reactance\".")))
		return false;

	if (!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlSat->GetValue(), data.satFactor,
		_("Value entered incorrectly in the field \"Saturation factor\".")))
		return false;

	if (!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlOCFreq->GetValue(), data.ocFrequency,
		_("Value entered incorrectly in the field \"Open-circuit frequency\".")))
		return false;

	if (!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlSyncXd->GetValue(), data.syncXd,
		_("Value entered incorrectly in the field \"Synchronous direct-axis reactance\".")))
		return false;

	if (!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlSyncXq->GetValue(), data.syncXq,
		_("Value entered incorrectly in the field \"Synchronous quadrature-axis reactance\".")))
		return false;

	if (!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlTranXd->GetValue(), data.transXd,
		_("Value entered incorrectly in the field \"Transitory direct-axis reactance\".")))
		return false;

	if (!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlTranXq->GetValue(), data.transXq,
		_("Value entered incorrectly in the field \"Transitory quadrature-axis reactance\".")))
		return false;

	if (!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlTranTd0->GetValue(), data.transTd0,
		_("Value entered incorrectly in the field \"Transitory direct-axis time constant\".")))
		return false;

	if (!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlTranTq0->GetValue(), data.transTq0,
		_("Value entered incorrectly in the field \"Transitory quadrature-axis time constant\".")))
		return false;

	if (!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlSubXd->GetValue(), data.subXd,
		_("Value entered incorrectly in the field \"Subtransitory direct-axis reactance\".")))
		return false;

	if (!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlSubXq->GetValue(), data.subXq,
		_("Value entered incorrectly in the field \"Subtransitory quadrature-axis reactance\".")))
		return false;

	if (!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlSubTd0->GetValue(), data.subTd0,
		_("Value entered incorrectly in the field \"Subtransitory direct-axis time constant\".")))
		return false;

	if (!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlSubTq0->GetValue(), data.subTq0,
		_("Value entered incorrectly in the field \"Subtransitory quadrature-axis time constant\".")))
		return false;

	m_syncGenerator->SetElectricalData(data);

	return true;
}
void GeneratorStabForm::UseAVRClick(wxCommandEvent& event)
{
	m_buttonEditAVR->Enable(m_checkBoxUseAVR->GetValue());
}
void GeneratorStabForm::UseSGClick(wxCommandEvent& event)
{
	m_buttonEditSG->Enable(m_checkBoxUseSG->GetValue());
}
