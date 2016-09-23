#include "GeneratorStabForm.h"
#include "SyncGenerator.h"

GeneratorStabForm::GeneratorStabForm(wxWindow* parent, SyncGenerator* syncGenerator) : GeneratorStabFormBase(parent)
{
    m_syncGenerator = syncGenerator;
    m_parent = parent;

    SyncGeneratorElectricalData data = syncGenerator->GetElectricalData();

    m_checkBoxPlotSyncMachine->SetValue(data.plotSyncMachine);

    m_textCtrlInertia->SetValue(wxString::FromDouble(data.inertia));
    m_textCtrlDamping->SetValue(wxString::FromDouble(data.damping));

    m_checkBoxUseAVR->SetValue(data.useAVR);
    m_buttonEditAVR->Enable(data.useAVR);

    m_checkBoxUseSG->SetValue(data.useSpeedGovernor);
    m_buttonEditSG->Enable(data.useSpeedGovernor);

    m_textCtrlRa->SetValue(wxString::FromDouble(data.armResistance));
    m_textCtrlXp->SetValue(wxString::FromDouble(data.potierReactance));
    m_textCtrlSat->SetValue(wxString::FromDouble(data.satFactor));

    m_textCtrlSyncXd->SetValue(wxString::FromDouble(data.syncXd));
    m_textCtrlSyncXq->SetValue(wxString::FromDouble(data.syncXq));

    m_textCtrlTranXd->SetValue(wxString::FromDouble(data.transXd));
    m_textCtrlTranXq->SetValue(wxString::FromDouble(data.transXq));
    m_textCtrlTranTd0->SetValue(wxString::FromDouble(data.transTd0));
    m_textCtrlTranTq0->SetValue(wxString::FromDouble(data.transTq0));

    m_textCtrlSubXd->SetValue(wxString::FromDouble(data.subXd));
    m_textCtrlSubXq->SetValue(wxString::FromDouble(data.subXq));
    m_textCtrlSubTd0->SetValue(wxString::FromDouble(data.subTd0));
    m_textCtrlSubTq0->SetValue(wxString::FromDouble(data.subTq0));
}

GeneratorStabForm::~GeneratorStabForm() {}
void GeneratorStabForm::OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
void GeneratorStabForm::OnEditAVRButtonClick(wxCommandEvent& event) {}
void GeneratorStabForm::OnOKButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

void GeneratorStabForm::OnSpeedGovernorButtonClick(wxCommandEvent& event) {}
void GeneratorStabForm::OnSwitchingButtonClick(wxCommandEvent& event) {}
bool GeneratorStabForm::ValidateData()
{
    SyncGeneratorElectricalData data = m_syncGenerator->GetElectricalData();

    data.plotSyncMachine = m_checkBoxPlotSyncMachine->GetValue();

    if(!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlInertia->GetValue(), data.inertia,
                                          _("Value entered incorrectly in the field \"Inertia\".")))
	return false;

    if(!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlDamping->GetValue(), data.damping,
                                          _("Value entered incorrectly in the field \"Damping factor\".")))
	return false;

    data.useAVR = m_checkBoxUseAVR->GetValue();
    data.useSpeedGovernor = m_checkBoxUseSG->GetValue();

    if(!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlRa->GetValue(), data.armResistance,
                                          _("Value entered incorrectly in the field \"Armature resistance\".")))
	return false;

    if(!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlXp->GetValue(), data.potierReactance,
                                          _("Value entered incorrectly in the field \"Potier reactance\".")))
	return false;

    if(!m_syncGenerator->DoubleFromString(m_parent, m_textCtrlSat->GetValue(), data.satFactor,
                                          _("Value entered incorrectly in the field \"Saturation factor\".")))
	return false;

    if(!m_syncGenerator->DoubleFromString(
           m_parent, m_textCtrlSyncXd->GetValue(), data.syncXd,
           _("Value entered incorrectly in the field \"Synchronous direct-axis reactance\".")))
	return false;

    if(!m_syncGenerator->DoubleFromString(
           m_parent, m_textCtrlSyncXq->GetValue(), data.syncXq,
           _("Value entered incorrectly in the field \"Synchronous quadrature-axis reactance\".")))
	return false;

    if(!m_syncGenerator->DoubleFromString(
           m_parent, m_textCtrlTranXd->GetValue(), data.transXd,
           _("Value entered incorrectly in the field \"Transitory direct-axis reactance\".")))
	return false;

    if(!m_syncGenerator->DoubleFromString(
           m_parent, m_textCtrlTranXq->GetValue(), data.transXq,
           _("Value entered incorrectly in the field \"Transitory quadrature-axis reactance\".")))
	return false;

    if(!m_syncGenerator->DoubleFromString(
           m_parent, m_textCtrlTranTd0->GetValue(), data.transTd0,
           _("Value entered incorrectly in the field \"Transitory direct-axis time constant\".")))
	return false;

    if(!m_syncGenerator->DoubleFromString(
           m_parent, m_textCtrlTranTq0->GetValue(), data.transTq0,
           _("Value entered incorrectly in the field \"Transitory quadrature-axis time constant\".")))
	return false;

    if(!m_syncGenerator->DoubleFromString(
           m_parent, m_textCtrlSubXd->GetValue(), data.subXd,
           _("Value entered incorrectly in the field \"Subtransitory direct-axis reactance\".")))
	return false;

    if(!m_syncGenerator->DoubleFromString(
           m_parent, m_textCtrlSubXq->GetValue(), data.subXq,
           _("Value entered incorrectly in the field \"Subtransitory quadrature-axis reactance\".")))
	return false;

    if(!m_syncGenerator->DoubleFromString(
           m_parent, m_textCtrlSubTd0->GetValue(), data.subTd0,
           _("Value entered incorrectly in the field \"Subtransitory direct-axis time constant\".")))
	return false;

    if(!m_syncGenerator->DoubleFromString(
           m_parent, m_textCtrlSubTq0->GetValue(), data.subTq0,
           _("Value entered incorrectly in the field \"Subtransitory quadrature-axis time constant\".")))
	return false;

    m_syncGenerator->SetElectricalData(data);

    return true;
}
void GeneratorStabForm::UseAVRClick(wxCommandEvent& event) { m_buttonEditAVR->Enable(m_checkBoxUseAVR->GetValue()); }
void GeneratorStabForm::UseSGClick(wxCommandEvent& event) { m_buttonEditSG->Enable(m_checkBoxUseSG->GetValue()); }
