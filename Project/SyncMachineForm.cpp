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
#include "SyncMachineForm.h"
#include "SyncGenerator.h"
#include "SyncMotor.h"

SyncMachineForm::SyncMachineForm(wxWindow* parent, SyncGenerator* syncGenerator, wxGLContext* sharedGLContext) : SyncMachineFormBase(parent)
{
    SetSize(GetBestSize());
    ReplaceStaticTextLabelChar(m_staticTextPosResistance, L'\u2081');
    ReplaceStaticTextLabelChar(m_staticTextPosReactance, L'\u2081');
    ReplaceStaticTextLabelChar(m_staticTextNegResistance, L'\u2082');
    ReplaceStaticTextLabelChar(m_staticTextNegReactance, L'\u2082');
    ReplaceStaticTextLabelChar(m_staticTextZeroResistance, L'\u2080');
    ReplaceStaticTextLabelChar(m_staticTextZeroReactance, L'\u2080');
    Layout();
    m_syncGenerator = syncGenerator;
    m_parent = parent;
    m_sharedGLContext = sharedGLContext;

    SyncGeneratorElectricalData data = syncGenerator->GetElectricalData();

    m_textCtrlName->SetValue(data.name);

    m_textCtrlnominalPower->SetValue(SyncGenerator::StringFromDouble(data.nominalPower));
    switch(data.nominalPowerUnit) {
        case ElectricalUnit::UNIT_VA:
            m_choiceNominalPower->SetSelection(0);
            break;
        case ElectricalUnit::UNIT_kVA:
            m_choiceNominalPower->SetSelection(1);
            break;
        case ElectricalUnit::UNIT_MVA:
            m_choiceNominalPower->SetSelection(2);
            break;
        default:
            break;
    }

    m_textCtrlActivePower->SetValue(SyncGenerator::StringFromDouble(data.activePower));
    switch(data.activePowerUnit) {
        case ElectricalUnit::UNIT_PU:
            m_choiceActivePower->SetSelection(0);
            break;
        case ElectricalUnit::UNIT_W:
            m_choiceActivePower->SetSelection(1);
            break;
        case ElectricalUnit::UNIT_kW:
            m_choiceActivePower->SetSelection(2);
            break;
        case ElectricalUnit::UNIT_MW:
            m_choiceActivePower->SetSelection(3);
            break;
        default:
            break;
    }

    m_textCtrlReactivePower->SetValue(SyncGenerator::StringFromDouble(data.reactivePower));
    switch(data.reactivePowerUnit) {
        case ElectricalUnit::UNIT_PU:
            m_choiceReactivePower->SetSelection(0);
            break;
        case ElectricalUnit::UNIT_var:
            m_choiceReactivePower->SetSelection(1);
            break;
        case ElectricalUnit::UNIT_kvar:
            m_choiceReactivePower->SetSelection(2);
            break;
        case ElectricalUnit::UNIT_Mvar:
            m_choiceReactivePower->SetSelection(3);
            break;
        default:
            break;
    }
    m_checkBoxMaxReactive->SetValue(data.haveMaxReactive);

    m_textCtrlMaxRectivePower->SetValue(SyncGenerator::StringFromDouble(data.maxReactive));
    switch(data.maxReactiveUnit) {
        case ElectricalUnit::UNIT_PU:
            m_choiceMaxRectivePower->SetSelection(0);
            break;
        case ElectricalUnit::UNIT_var:
            m_choiceMaxRectivePower->SetSelection(1);
            break;
        case ElectricalUnit::UNIT_kvar:
            m_choiceMaxRectivePower->SetSelection(2);
            break;
        case ElectricalUnit::UNIT_Mvar:
            m_choiceMaxRectivePower->SetSelection(3);
            break;
        default:
            break;
    }
    m_textCtrlMaxRectivePower->Enable(data.haveMaxReactive);
    m_choiceMaxRectivePower->Enable(data.haveMaxReactive);

    m_checkBoxMinReactive->SetValue(data.haveMinReactive);
    m_textCtrlMinRectivePower->SetValue(SyncGenerator::StringFromDouble(data.minReactive));
    switch(data.minReactiveUnit) {
        case ElectricalUnit::UNIT_PU:
            m_choiceMinRectivePower->SetSelection(0);
            break;
        case ElectricalUnit::UNIT_var:
            m_choiceMinRectivePower->SetSelection(1);
            break;
        case ElectricalUnit::UNIT_kvar:
            m_choiceMinRectivePower->SetSelection(2);
            break;
        case ElectricalUnit::UNIT_Mvar:
            m_choiceMinRectivePower->SetSelection(3);
            break;
        default:
            break;
    }
    m_textCtrlMinRectivePower->Enable(data.haveMinReactive);
    m_choiceMinRectivePower->Enable(data.haveMinReactive);

    m_checkBoxUseMachinePower->SetValue(data.useMachineBase);

    m_textCtrlPosResistance->SetValue(SyncGenerator::StringFromDouble(data.positiveResistance));
    m_textCtrlPosReactance->SetValue(SyncGenerator::StringFromDouble(data.positiveReactance));
    m_textCtrlNegResistance->SetValue(SyncGenerator::StringFromDouble(data.negativeResistance));
    m_textCtrlNegReactance->SetValue(SyncGenerator::StringFromDouble(data.negativeReactance));
    m_textCtrlZeroResistance->SetValue(SyncGenerator::StringFromDouble(data.zeroResistance));
    m_textCtrlZeroReactance->SetValue(SyncGenerator::StringFromDouble(data.zeroReactance));
    m_textCtrlGrdResistance->SetValue(SyncGenerator::StringFromDouble(data.groundResistance));
    m_textCtrlGrdReactance->SetValue(SyncGenerator::StringFromDouble(data.groundReactance));
    m_checkBoxGroundNeutral->SetValue(data.groundNeutral);
}

SyncMachineForm::SyncMachineForm(wxWindow* parent, SyncMotor* syncMotor) : SyncMachineFormBase(parent)
{
    m_buttonStab->Enable(false);
    SetSize(GetBestSize());
    ReplaceStaticTextLabelChar(m_staticTextPosResistance, L'\u2081');
    ReplaceStaticTextLabelChar(m_staticTextPosReactance, L'\u2081');
    ReplaceStaticTextLabelChar(m_staticTextNegResistance, L'\u2082');
    ReplaceStaticTextLabelChar(m_staticTextNegReactance, L'\u2082');
    ReplaceStaticTextLabelChar(m_staticTextZeroResistance, L'\u2080');
    ReplaceStaticTextLabelChar(m_staticTextZeroReactance, L'\u2080');
    Layout();
    m_syncMotor = syncMotor;
    m_parent = parent;

    SyncMotorElectricalData data = syncMotor->GetElectricalData();

    m_textCtrlName->SetValue(data.name);

    m_textCtrlnominalPower->SetValue(SyncMotor::StringFromDouble(data.nominalPower));
    switch(data.nominalPowerUnit) {
        case ElectricalUnit::UNIT_VA:
            m_choiceNominalPower->SetSelection(0);
            break;
        case ElectricalUnit::UNIT_kVA:
            m_choiceNominalPower->SetSelection(1);
            break;
        case ElectricalUnit::UNIT_MVA:
            m_choiceNominalPower->SetSelection(2);
            break;
        default:
            break;
    }

    m_textCtrlActivePower->SetValue(SyncMotor::StringFromDouble(data.activePower));
    switch(data.activePowerUnit) {
        case ElectricalUnit::UNIT_PU:
            m_choiceActivePower->SetSelection(0);
            break;
        case ElectricalUnit::UNIT_W:
            m_choiceActivePower->SetSelection(1);
            break;
        case ElectricalUnit::UNIT_kW:
            m_choiceActivePower->SetSelection(2);
            break;
        case ElectricalUnit::UNIT_MW:
            m_choiceActivePower->SetSelection(3);
            break;
        default:
            break;
    }

    m_textCtrlReactivePower->SetValue(SyncMotor::StringFromDouble(data.reactivePower));
    switch(data.reactivePowerUnit) {
        case ElectricalUnit::UNIT_PU:
            m_choiceReactivePower->SetSelection(0);
            break;
        case ElectricalUnit::UNIT_var:
            m_choiceReactivePower->SetSelection(1);
            break;
        case ElectricalUnit::UNIT_kvar:
            m_choiceReactivePower->SetSelection(2);
            break;
        case ElectricalUnit::UNIT_Mvar:
            m_choiceReactivePower->SetSelection(3);
            break;
        default:
            break;
    }
    m_checkBoxMaxReactive->SetValue(data.haveMaxReactive);

    m_textCtrlMaxRectivePower->SetValue(SyncMotor::StringFromDouble(data.maxReactive));
    switch(data.maxReactiveUnit) {
        case ElectricalUnit::UNIT_PU:
            m_choiceMaxRectivePower->SetSelection(0);
            break;
        case ElectricalUnit::UNIT_var:
            m_choiceMaxRectivePower->SetSelection(1);
            break;
        case ElectricalUnit::UNIT_kvar:
            m_choiceMaxRectivePower->SetSelection(2);
            break;
        case ElectricalUnit::UNIT_Mvar:
            m_choiceMaxRectivePower->SetSelection(3);
            break;
        default:
            break;
    }
    m_textCtrlMaxRectivePower->Enable(data.haveMaxReactive);
    m_choiceMaxRectivePower->Enable(data.haveMaxReactive);

    m_checkBoxMinReactive->SetValue(data.haveMinReactive);
    m_textCtrlMinRectivePower->SetValue(SyncMotor::StringFromDouble(data.minReactive));
    switch(data.minReactiveUnit) {
        case ElectricalUnit::UNIT_PU:
            m_choiceMinRectivePower->SetSelection(0);
            break;
        case ElectricalUnit::UNIT_var:
            m_choiceMinRectivePower->SetSelection(1);
            break;
        case ElectricalUnit::UNIT_kvar:
            m_choiceMinRectivePower->SetSelection(2);
            break;
        case ElectricalUnit::UNIT_Mvar:
            m_choiceMinRectivePower->SetSelection(3);
            break;
        default:
            break;
    }
    m_textCtrlMinRectivePower->Enable(data.haveMinReactive);
    m_choiceMinRectivePower->Enable(data.haveMinReactive);

    m_checkBoxUseMachinePower->SetValue(data.useMachineBase);

    m_textCtrlPosResistance->SetValue(SyncMotor::StringFromDouble(data.positiveResistance));
    m_textCtrlPosReactance->SetValue(SyncMotor::StringFromDouble(data.positiveReactance));
    m_textCtrlNegResistance->SetValue(SyncMotor::StringFromDouble(data.negativeResistance));
    m_textCtrlNegReactance->SetValue(SyncMotor::StringFromDouble(data.negativeReactance));
    m_textCtrlZeroResistance->SetValue(SyncMotor::StringFromDouble(data.zeroResistance));
    m_textCtrlZeroReactance->SetValue(SyncMotor::StringFromDouble(data.zeroReactance));
    m_textCtrlGrdResistance->SetValue(SyncMotor::StringFromDouble(data.groundResistance));
    m_textCtrlGrdReactance->SetValue(SyncMotor::StringFromDouble(data.groundReactance));
    m_checkBoxGroundNeutral->SetValue(data.groundNeutral);
}

SyncMachineForm::~SyncMachineForm() {}
void SyncMachineForm::OnCheckMaxReactive(wxCommandEvent& event)
{
    m_textCtrlMaxRectivePower->Enable(m_checkBoxMaxReactive->GetValue());
    m_choiceMaxRectivePower->Enable(m_checkBoxMaxReactive->GetValue());
}
void SyncMachineForm::OnCheckMinReactive(wxCommandEvent& event)
{
    m_textCtrlMinRectivePower->Enable(m_checkBoxMinReactive->GetValue());
    m_choiceMinRectivePower->Enable(m_checkBoxMinReactive->GetValue());
}
void SyncMachineForm::OnOKButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}
void SyncMachineForm::OnStabilityButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) {
        if(m_syncGenerator) {
            GeneratorStabForm* stabForm = new GeneratorStabForm(m_parent, m_syncGenerator, m_sharedGLContext);
            if(stabForm->ShowModal() == wxID_OK) {
                stabForm->Destroy();
                EndModal(wxID_OK);
            }

            stabForm->Destroy();
            EndModal(wxID_CANCEL);
        }
    }
}

bool SyncMachineForm::ValidateData()
{
    if(m_syncGenerator) {
        SyncGeneratorElectricalData data = m_syncGenerator->GetElectricalData();
        data.name = m_textCtrlName->GetValue();

        if(!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlnominalPower->GetValue(), data.nominalPower,
                                              _("Value entered incorrectly in the field \"Nominal power\".")))
            return false;
        switch(m_choiceNominalPower->GetSelection()) {
            case 0:
                data.nominalPowerUnit = ElectricalUnit::UNIT_VA;
                break;
            case 1:
                data.nominalPowerUnit = ElectricalUnit::UNIT_kVA;
                break;
            case 2:
                data.nominalPowerUnit = ElectricalUnit::UNIT_MVA;
                break;
        }

        if(!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlActivePower->GetValue(), data.activePower,
                                              _("Value entered incorrectly in the field \"Active power\".")))
            return false;
        switch(m_choiceActivePower->GetSelection()) {
            case 0:
                data.activePowerUnit = ElectricalUnit::UNIT_PU;
                break;
            case 1:
                data.activePowerUnit = ElectricalUnit::UNIT_W;
                break;
            case 2:
                data.activePowerUnit = ElectricalUnit::UNIT_kW;
                break;
            case 3:
                data.activePowerUnit = ElectricalUnit::UNIT_MW;
                break;
        }

        if(!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlReactivePower->GetValue(), data.reactivePower,
                                              _("Value entered incorrectly in the field \"Reactive power\".")))
            return false;
        switch(m_choiceReactivePower->GetSelection()) {
            case 0:
                data.reactivePowerUnit = ElectricalUnit::UNIT_PU;
                break;
            case 1:
                data.reactivePowerUnit = ElectricalUnit::UNIT_var;
                break;
            case 2:
                data.reactivePowerUnit = ElectricalUnit::UNIT_kvar;
                break;
            case 3:
                data.reactivePowerUnit = ElectricalUnit::UNIT_Mvar;
                break;
        }

        data.haveMaxReactive = m_checkBoxMaxReactive->GetValue();
        if(data.haveMaxReactive) {
            if(!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlMaxRectivePower->GetValue(), data.maxReactive,
                                                  _("Value entered incorrectly in the field \"Max reactive power\".")))
                return false;
            switch(m_choiceMaxRectivePower->GetSelection()) {
                case 0:
                    data.maxReactiveUnit = ElectricalUnit::UNIT_PU;
                    break;
                case 1:
                    data.maxReactiveUnit = ElectricalUnit::UNIT_var;
                    break;
                case 2:
                    data.maxReactiveUnit = ElectricalUnit::UNIT_kvar;
                    break;
                case 3:
                    data.maxReactiveUnit = ElectricalUnit::UNIT_Mvar;
                    break;
            }
        }

        data.haveMinReactive = m_checkBoxMinReactive->GetValue();
        if(data.haveMinReactive) {
            if(!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlMinRectivePower->GetValue(), data.minReactive,
                                                  _("Value entered incorrectly in the field \"Min reactive power\".")))
                return false;
            switch(m_choiceMinRectivePower->GetSelection()) {
                case 0:
                    data.minReactiveUnit = ElectricalUnit::UNIT_PU;
                    break;
                case 1:
                    data.minReactiveUnit = ElectricalUnit::UNIT_var;
                    break;
                case 2:
                    data.minReactiveUnit = ElectricalUnit::UNIT_kvar;
                    break;
                case 3:
                    data.minReactiveUnit = ElectricalUnit::UNIT_Mvar;
                    break;
            }
        }

        data.useMachineBase = m_checkBoxUseMachinePower->GetValue();

        if(!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlPosResistance->GetValue(), data.positiveResistance,
                                              _("Value entered incorrectly in the field \"Positive resistance\".")))
            return false;

        if(!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlPosReactance->GetValue(), data.positiveReactance,
                                              _("Value entered incorrectly in the field \"Positive reactance\".")))
            return false;

        if(!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlNegResistance->GetValue(), data.negativeResistance,
                                              _("Value entered incorrectly in the field \"Negative resistance\".")))
            return false;

        if(!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlNegReactance->GetValue(), data.negativeReactance,
                                              _("Value entered incorrectly in the field \"Negative reactance\".")))
            return false;

        if(!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlZeroResistance->GetValue(), data.zeroResistance,
                                              _("Value entered incorrectly in the field \"Zero resistance\".")))
            return false;

        if(!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlZeroReactance->GetValue(), data.zeroReactance,
                                              _("Value entered incorrectly in the field \"Zero reactance\".")))
            return false;

        if(!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlGrdResistance->GetValue(), data.groundResistance,
                                              _("Value entered incorrectly in the field \"Ground resistance\".")))
            return false;

        if(!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlGrdReactance->GetValue(), data.groundReactance,
                                              _("Value entered incorrectly in the field \"Ground reactance\".")))
            return false;

        data.groundNeutral = m_checkBoxGroundNeutral->GetValue();

        m_syncGenerator->SetElectricalData(data);
    } else if(m_syncMotor) {
        SyncMotorElectricalData data = m_syncMotor->GetElectricalData();
        data.name = m_textCtrlName->GetValue();

        if(!m_syncMotor->DoubleFromString(m_parent, m_textCtrlnominalPower->GetValue(), data.nominalPower,
                                          _("Value entered incorrectly in the field \"Nominal power\".")))
            return false;
        switch(m_choiceNominalPower->GetSelection()) {
            case 0:
                data.nominalPowerUnit = ElectricalUnit::UNIT_VA;
                break;
            case 1:
                data.nominalPowerUnit = ElectricalUnit::UNIT_kVA;
                break;
            case 2:
                data.nominalPowerUnit = ElectricalUnit::UNIT_MVA;
                break;
        }

        if(!m_syncMotor->DoubleFromString(m_parent, m_textCtrlActivePower->GetValue(), data.activePower,
                                          _("Value entered incorrectly in the field \"Active power\".")))
            return false;
        switch(m_choiceActivePower->GetSelection()) {
            case 0:
                data.activePowerUnit = ElectricalUnit::UNIT_PU;
                break;
            case 1:
                data.activePowerUnit = ElectricalUnit::UNIT_W;
                break;
            case 2:
                data.activePowerUnit = ElectricalUnit::UNIT_kW;
                break;
            case 3:
                data.activePowerUnit = ElectricalUnit::UNIT_MW;
                break;
        }

        if(!m_syncMotor->DoubleFromString(m_parent, m_textCtrlReactivePower->GetValue(), data.reactivePower,
                                          _("Value entered incorrectly in the field \"Reactive power\".")))
            return false;
        switch(m_choiceReactivePower->GetSelection()) {
            case 0:
                data.reactivePowerUnit = ElectricalUnit::UNIT_PU;
                break;
            case 1:
                data.reactivePowerUnit = ElectricalUnit::UNIT_var;
                break;
            case 2:
                data.reactivePowerUnit = ElectricalUnit::UNIT_kvar;
                break;
            case 3:
                data.reactivePowerUnit = ElectricalUnit::UNIT_Mvar;
                break;
        }

        data.haveMaxReactive = m_checkBoxMaxReactive->GetValue();
        if(data.haveMaxReactive) {
            if(!m_syncMotor->DoubleFromString(m_parent, m_textCtrlMaxRectivePower->GetValue(), data.maxReactive,
                                              _("Value entered incorrectly in the field \"Max reactive power\".")))
                return false;
            switch(m_choiceMaxRectivePower->GetSelection()) {
                case 0:
                    data.maxReactiveUnit = ElectricalUnit::UNIT_PU;
                    break;
                case 1:
                    data.maxReactiveUnit = ElectricalUnit::UNIT_var;
                    break;
                case 2:
                    data.maxReactiveUnit = ElectricalUnit::UNIT_kvar;
                    break;
                case 3:
                    data.maxReactiveUnit = ElectricalUnit::UNIT_Mvar;
                    break;
            }
        }

        data.haveMinReactive = m_checkBoxMinReactive->GetValue();
        if(data.haveMinReactive) {
            if(!m_syncMotor->DoubleFromString(m_parent, m_textCtrlMinRectivePower->GetValue(), data.minReactive,
                                              _("Value entered incorrectly in the field \"Min reactive power\".")))
                return false;
            switch(m_choiceMinRectivePower->GetSelection()) {
                case 0:
                    data.minReactiveUnit = ElectricalUnit::UNIT_PU;
                    break;
                case 1:
                    data.minReactiveUnit = ElectricalUnit::UNIT_var;
                    break;
                case 2:
                    data.minReactiveUnit = ElectricalUnit::UNIT_kvar;
                    break;
                case 3:
                    data.minReactiveUnit = ElectricalUnit::UNIT_Mvar;
                    break;
            }
        }

        data.useMachineBase = m_checkBoxUseMachinePower->GetValue();

        if(!m_syncMotor->DoubleFromString(m_parent, m_textCtrlPosResistance->GetValue(), data.positiveResistance,
                                          _("Value entered incorrectly in the field \"Positive resistance\".")))
            return false;

        if(!m_syncMotor->DoubleFromString(m_parent, m_textCtrlPosReactance->GetValue(), data.positiveReactance,
                                          _("Value entered incorrectly in the field \"Positive reactance\".")))
            return false;

        if(!m_syncMotor->DoubleFromString(m_parent, m_textCtrlNegResistance->GetValue(), data.negativeResistance,
                                          _("Value entered incorrectly in the field \"Negative resistance\".")))
            return false;

        if(!m_syncMotor->DoubleFromString(m_parent, m_textCtrlNegReactance->GetValue(), data.negativeReactance,
                                          _("Value entered incorrectly in the field \"Negative reactance\".")))
            return false;

        if(!m_syncMotor->DoubleFromString(m_parent, m_textCtrlZeroResistance->GetValue(), data.zeroResistance,
                                          _("Value entered incorrectly in the field \"Zero resistance\".")))
            return false;

        if(!m_syncMotor->DoubleFromString(m_parent, m_textCtrlZeroReactance->GetValue(), data.zeroReactance,
                                          _("Value entered incorrectly in the field \"Zero reactance\".")))
            return false;

        if(!m_syncMotor->DoubleFromString(m_parent, m_textCtrlGrdResistance->GetValue(), data.groundResistance,
                                          _("Value entered incorrectly in the field \"Ground resistance\".")))
            return false;

        if(!m_syncMotor->DoubleFromString(m_parent, m_textCtrlGrdReactance->GetValue(), data.groundReactance,
                                          _("Value entered incorrectly in the field \"Ground reactance\".")))
            return false;

        data.groundNeutral = m_checkBoxGroundNeutral->GetValue();

        m_syncMotor->SetElectricalData(data);
    }
    return true;
}

void SyncMachineForm::ReplaceStaticTextLabelChar(wxStaticText* staticText, wchar_t newChar)
{
    wxString label = staticText->GetLabel();
    label[label.length() - 2] = newChar;
    staticText->SetLabel(label);
}
