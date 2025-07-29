#include "EMTElementForm.h"
#include "../elements/powerElement/EMTElement.h"
#include "../editors/ChartView.h"
#include "../editors/Workspace.h"
#include "../extLibs/fftw/fftw3.h"
#include "ATPFileEditorForm.h"

#include <wx/wfstream.h>
#include <wx/datstrm.h>
#include <wx/dir.h>
#include <wx/process.h>

EMTElementForm::EMTElementForm(wxWindow* parent, EMTElement* emtElement)
	: EMTElementFormBase(parent), m_parent(parent), m_emtElement(emtElement)
{
	m_staticTextNode->SetLabel(m_staticTextNode->GetLabel() + wxT(" (3\u0278)"));

	auto data = m_emtElement->GetEMTElementData();
	m_textCtrlName->SetValue(data.name);
	m_filePickerATPFile->SetPath(data.atpFile.GetFullPath());
	LoadNodes(data.atpFile.GetFullPath());
	m_choiceNodes->SetSelection(m_choiceNodes->FindString(data.atpNodeName));
	m_textCtrlStepSize->SetValue(wxString::Format(wxT("%e"), data.stepSize));
	m_spinCtrlCyclesToSS->SetValue(data.cyclesToSS);
	m_textCtrlRecFreq->SetValue(wxString::Format(wxT("%d"), data.recordFrequency));
	m_checkBoxUseFilter->SetValue(data.useMedianFilter);
	m_spinCtrlMaxNumHarmonics->SetValue(data.numMaxHarmonics);
	m_textCtrlHarmThreshold->SetValue(wxString::Format(wxT("%.2f"), data.harmonicsThreshold));

	m_properties = static_cast<Workspace*>(parent)->GetProperties();

	SetSize(DoGetBestSize());
}

EMTElementForm::~EMTElementForm()
{
}

bool EMTElementForm::ValidateData()
{
	wxFileName fileName(m_filePickerATPFile->GetPath());
	if (!fileName.IsOk()) {
		wxMessageDialog msgDialog(m_parent, _("The ATP file doesn't exist. Insert the correct path."), _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
		msgDialog.ShowModal();
		return false;
	}
	double stepSize = 0.0;
	if (!m_textCtrlStepSize->GetValue().ToDouble(&stepSize)) {
		wxMessageDialog msgDialog(m_parent, _("The step size must be a number."), _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
		msgDialog.ShowModal();
		return false;
	}
	long recFreq = 0;
	if (!m_textCtrlRecFreq->GetValue().ToLong(&recFreq)) {
		wxMessageDialog msgDialog(m_parent, _("The steps to record data must be an integer number."), _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
		msgDialog.ShowModal();
		return false;
	}
	if (m_choiceNodes->GetSelection() == wxNOT_FOUND) {
		wxMessageDialog msgDialog(m_parent, _("No connection node selected."), _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
		msgDialog.ShowModal();
		return false;
	}
	double harmThreshold = 0.0;
	if (!m_textCtrlHarmThreshold->GetValue().ToDouble(&harmThreshold)) {
		wxMessageDialog msgDialog(m_parent, _("The threshold to detect harmonics must be a number."), _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
		msgDialog.ShowModal();
		return false;
	}

	auto data = m_emtElement->GetEMTElementData();
	data.name = m_textCtrlName->GetValue();
	data.atpFile = m_filePickerATPFile->GetPath();
	data.atpNodeName = m_choiceNodes->GetString(m_choiceNodes->GetSelection());
	data.stepSize = stepSize;
	data.cyclesToSS = m_spinCtrlCyclesToSS->GetValue();
	data.recordFrequency = recFreq;
	data.useMedianFilter = m_checkBoxUseFilter->GetValue();
	data.numMaxHarmonics = m_spinCtrlMaxNumHarmonics->GetValue();
	data.harmonicsThreshold = harmThreshold;

	m_emtElement->SetEMTElementData(data);

	return true;
}

void EMTElementForm::OnCancelClick(wxCommandEvent& event)
{
	EndModal(wxID_CANCEL);
}
void EMTElementForm::OnOKClick(wxCommandEvent& event)
{
	if (ValidateData()) EndModal(wxID_OK);
}

void EMTElementForm::LoadNodes(wxString atpFilePath)
{
	wxArrayString atpFile;
	wxTextFile file(atpFilePath);
	if (!file.Exists()) return;

	if (file.Open(atpFilePath)) {
		atpFile.Add(file.GetFirstLine());
		while (!file.Eof())
		{
			atpFile.Add(file.GetNextLine());
		}
		file.Close();
	}
	wxArrayString nodeList = m_emtElement->GetATPNodes(atpFile);
	m_choiceNodes->Clear();
	m_choiceNodes->Append(nodeList);
}

void EMTElementForm::OnEditATPFileClick(wxCommandEvent& event)
{
	wxFileName fileName(m_filePickerATPFile->GetPath());
	if (!fileName.IsOk()) {
		wxMessageDialog msgDialog(m_parent, _("The ATP file doesn't exist. Insert the correct path."), _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
		msgDialog.ShowModal();
		return;
	}
	ATPFileEditorForm atpFileEditorForm(m_parent, m_filePickerATPFile->GetPath());
	atpFileEditorForm.SetSize(800, 600);
	atpFileEditorForm.ShowModal();
}

void EMTElementForm::OnTestClick(wxCommandEvent& event)
{
	if (ValidateData()) {
		wxString errorMsg;
		m_emtElement->UpdateData(m_properties);
		if (!m_emtElement->CalculateCurrent(errorMsg, true)) {
			wxMessageDialog msgDialog(m_parent, errorMsg, _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
			msgDialog.ShowModal();
			return;
		}
		std::vector<double> time, in, timeSamp, inSamp, outR, outI, outM, outP, freq;
		auto atpData = m_emtElement->GetEMTElementData().atpData;
		auto inFFTData = m_emtElement->GetEMTElementData().inFFTData;
		auto outFFTData = m_emtElement->GetEMTElementData().outFFTData;
		for (auto data : atpData) {
			time.emplace_back(data.first);
			in.emplace_back(data.second);
		}
		for (auto data : inFFTData) {
			timeSamp.emplace_back(data.first);
			inSamp.emplace_back(data.second);
		}
		for (auto data : outFFTData) {
			outR.emplace_back(data.second.real());
			outI.emplace_back(data.second.imag());
			outM.emplace_back(abs(data.second));
			outP.emplace_back(arg(data.second) * 180.0 / M_PI);
			freq.emplace_back(data.first);
		}


		std::vector<double> currMag, currPha, harmOrder;
		auto harmData = m_emtElement->GetEMTElementData().currHarmonics;
		//auto harmOrderData = m_emtElement->GetEMTElementData().currHarmonicsOrder;
		//for (size_t i = 0; i < harmData.size(); i++) {
		//	currMag.emplace_back(abs(harmData[i]));
		//	currPha.emplace_back(arg(harmData[i]) * 180.0 / M_PI);
		//	harmOrder.emplace_back(static_cast<double>(harmOrderData[i]));
		//}
		for(auto const& data : harmData){
			currMag.emplace_back(abs(data.second));
			currPha.emplace_back(arg(data.second) * 180.0 / M_PI);
			harmOrder.emplace_back(static_cast<double>(data.first));
		}

		std::vector<ElementPlotData> plotDataList;
		ElementPlotData plotATPData(_("ATP Output"), ElementPlotData::CurveType::CT_TEST);
		ElementPlotData plotFFTData(_("Fourier Analysis"), ElementPlotData::CurveType::CT_TEST);
		ElementPlotData plotCurrData(_("Current Phasor"), ElementPlotData::CurveType::CT_TEST);

		plotATPData.AddData(in, _("Data"));

		plotFFTData.AddData(inSamp, _("Input"));
		plotFFTData.AddData(timeSamp, _("Time Sampled"));
		plotFFTData.AddData(outR, _("Real"));
		plotFFTData.AddData(outI, _("Imaginary"));
		plotFFTData.AddData(outM, _("Magnitude"));
		plotFFTData.AddData(outP, _("Phase"));
		plotFFTData.AddData(freq, _("Frequency"));

		plotCurrData.AddData(currMag, _("Magnitude"));
		plotCurrData.AddData(currPha, _("Phase"));
		plotCurrData.AddData(harmOrder, _("Harmonic Order"));

		plotDataList.push_back(plotATPData);
		plotDataList.push_back(plotFFTData);
		plotDataList.push_back(plotCurrData);
		ChartView* cView = new ChartView(this, plotDataList, time, m_properties->GetGeneralPropertiesData().plotLib);
		cView->Show();
	}
}
void EMTElementForm::OnATPFileChange(wxFileDirPickerEvent& event)
{
	LoadNodes(m_filePickerATPFile->GetPath());
}
