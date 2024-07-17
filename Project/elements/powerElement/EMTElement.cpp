#include "EMTElement.h"

#include "../../forms/EMTElementForm.h"
#include "../../utils/PropertiesData.h"
#include "../extLibs/fftw/fftw3.h"

#include <wx/dcgraph.h>
#include <wx/textfile.h>
#include <wx/dir.h>
#include <wx/process.h>
#include <wx/wfstream.h>
#include <wx/datstrm.h>

EMTElement::EMTElement()
{
}

EMTElement::EMTElement(wxString name)
{
	m_data.name = name;
}

EMTElement::~EMTElement()
{
}

Element* EMTElement::GetCopy()
{
	EMTElement* copy = new EMTElement();
	*copy = *this;
	return copy;
}

bool EMTElement::AddParent(Element* parent, wxPoint2DDouble position)
{
	if (parent) {
		m_parentList.push_back(parent);
		parent->AddChild(this);
		wxPoint2DDouble parentPt =
			parent->RotateAtPosition(position, -parent->GetAngle());        // Rotate click to horizontal position.
		parentPt.m_y = parent->GetPosition().m_y;                           // Centralize on bus.
		parentPt = parent->RotateAtPosition(parentPt, parent->GetAngle());  // Rotate back.

		m_position = parentPt + wxPoint2DDouble(0.0, 100.0);  // Shifts the position to the down of the bus.
		m_width = 50;
		m_height = 50;
		m_rect = wxRect2DDouble(m_position.m_x - m_width / 2.0, m_position.m_y - m_height / 2.0, m_width, m_height);

		m_pointList.push_back(parentPt);
		m_pointList.push_back(GetSwitchPoint(parent, parentPt, m_position));
		m_pointList.push_back(m_position + wxPoint2DDouble(0.0, -m_height / 2.0 - 10.0));
		m_pointList.push_back(m_position + wxPoint2DDouble(0.0, -m_height / 2.0));

		m_inserted = true;

		wxRect2DDouble genRect(0, 0, 0, 0);
		m_switchRect.push_back(genRect);  // Push a general rectangle.
		UpdateSwitches();

		// Base data
		auto data = static_cast<Bus*>(parent)->GetElectricalData();
		m_data.puVoltage = data.voltage;
		m_data.baseVoltage = GetValueFromUnit(data.nominalVoltage, data.nominalVoltageUnit) * (std::sqrt(2) / std::sqrt(3)); // phase-ground, peak value
		m_data.frequency = data.stabFreq;

		return true;
	}
	return false;
}

void EMTElement::DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const
{
	wxColour elementColour;
	if (m_online) {
		if (m_dynEvent)
			elementColour = m_dynamicEventColour;
		else
			elementColour = m_onlineElementColour;
	}
	else
		elementColour = m_offlineElementColour;

	if (m_inserted) {

		if (m_selected) {
			gc->SetPen(wxPen(wxColour(m_selectionColour), 2 + m_borderSize * 2.0));
			gc->SetBrush(*wxTRANSPARENT_BRUSH);

			gc->DrawLines(m_pointList.size(), &m_pointList[0]);

			// Push the current matrix on stack.
			gc->PushState();
			// Rotate the matrix around the object position.
			gc->Translate(m_position.m_x, m_position.m_y);
			gc->Rotate(wxDegToRad(m_angle));
			gc->Translate(-m_position.m_x, -m_position.m_y);

			//gc->DrawRectangle(m_position.m_x - m_width / 2.0, m_position.m_y - m_height / 2.0, m_width, m_height);
			gc->DrawRoundedRectangle(m_position.m_x - m_width / 2.0, m_position.m_y - m_height / 2.0, m_width, m_height, 10.0);

			gc->PopState();

			// Draw node selection.
			gc->SetPen(*wxTRANSPARENT_PEN);
			gc->SetBrush(wxBrush(wxColour(m_selectionColour)));
			DrawDCCircle(m_pointList[0], 5.0 + m_borderSize / scale, 10, gc);
		}
		// Draw EMTElement (layer 2).
		// Draw node.
		gc->SetPen(*wxTRANSPARENT_PEN);
		gc->SetBrush(wxBrush(wxColour(elementColour)));
		DrawDCCircle(m_pointList[0], 5.0, 10, gc);

		gc->SetPen(wxPen(wxColour(elementColour), 2));
		gc->SetBrush(*wxTRANSPARENT_BRUSH);
		gc->DrawLines(m_pointList.size(), &m_pointList[0]);

		DrawDCSwitches(gc);

		// Push the current matrix on stack.
		gc->PushState();
		// Rotate the matrix around the object position.
		gc->Translate(m_position.m_x, m_position.m_y);
		gc->Rotate(wxDegToRad(m_angle));
		gc->Translate(-m_position.m_x, -m_position.m_y);

		gc->SetPen(wxPen(wxColour(elementColour), 2));
		gc->SetBrush(*wxWHITE_BRUSH);
		//gc->DrawRectangle(m_position.m_x - m_width / 2.0, m_position.m_y - m_height / 2.0, m_width, m_height);
		gc->DrawRoundedRectangle(m_position.m_x - m_width / 2.0, m_position.m_y - m_height / 2.0, m_width, m_height, 10.0);

		//gc->SetPen(*wxBLACK_PEN);
		//wxFont font(10, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
		wxFont font;
		font.SetFaceName(wxT("CMU Serif"));
		font.SetPointSize(10);
		font.MakeBold();
		gc->SetFont(font, m_online ? wxColour(255, 60, 0) : m_offlineElementColour);
		double textWidth, textHeight;
		gc->GetTextExtent(_("EMT"), &textWidth, &textHeight);
		gc->DrawText(_("EMT"), m_position.m_x - textWidth / 2.0, m_position.m_y - textHeight / 2.0 + 20.0);

		wxGCDC* gcdc = new wxGCDC(gc);
		gcdc->SetPen(wxPen(m_online ? wxColour(0, 60, 255) : m_offlineElementColour, 2));
		std::vector<wxPoint> ptList;
		for (double x = m_position.m_x - m_width / 2.0 + 2; x < (m_position.m_x + m_width / 2.0); x += (m_width - 4.0) / 6.0) {
			ptList.emplace_back(x, m_position.m_y + std::sin((x - (m_position.m_x - m_width / 2.0 + 2)) / m_width * 6 * M_PI) * 20.0);
		}
		gcdc->DrawSpline(ptList.size(), ptList.data());

		gc->PopState();
	}
}

void EMTElement::Rotate(bool clockwise)
{
	double rotAngle = m_rotationAngle;
	if (!clockwise) rotAngle = -m_rotationAngle;

	m_angle += rotAngle;
	if (m_angle >= 360 || m_angle <= -360) m_angle = 0.0;
	m_pointList[2] = RotateAtPosition(m_pointList[2], rotAngle);
	m_pointList[3] = RotateAtPosition(m_pointList[3], rotAngle);
	UpdateSwitchesPosition();
}

bool EMTElement::GetContextMenu(wxMenu& menu)
{
	menu.Append(ID_EDIT_ELEMENT, _("Edit Electromagnetic Transient"));
	GeneralMenuItens(menu);
	return true;
}

bool EMTElement::ShowForm(wxWindow* parent, Element* element)
{
	EMTElementForm* emtForm = new EMTElementForm(parent, this);
	emtForm->SetTitle(_("Electromagnetic Transient"));
	if (emtForm->ShowModal() == wxID_OK) {
		emtForm->Destroy();
		return true;
	}
	emtForm->Destroy();
	return false;
}

rapidxml::xml_node<>* EMTElement::SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode)
{
	auto elementNode = XMLParser::AppendNode(doc, elementListNode, "EMTElement");
	XMLParser::SetNodeAttribute(doc, elementNode, "ID", m_elementID);

	SaveCADProperties(doc, elementNode);

	auto electricalProp = XMLParser::AppendNode(doc, elementNode, "ElectricalProperties");
	auto isOnline = XMLParser::AppendNode(doc, electricalProp, "IsOnline");
	XMLParser::SetNodeValue(doc, isOnline, m_online);
	auto name = XMLParser::AppendNode(doc, electricalProp, "Name");
	XMLParser::SetNodeValue(doc, name, m_data.name);
	auto atpFilePath = XMLParser::AppendNode(doc, electricalProp, "ATPFilePath");
	XMLParser::SetNodeValue(doc, atpFilePath, m_data.atpFile.GetFullPath());
	auto atpNodeName = XMLParser::AppendNode(doc, electricalProp, "ATPNodeName");
	XMLParser::SetNodeValue(doc, atpNodeName, m_data.atpNodeName);
	auto stepSize = XMLParser::AppendNode(doc, electricalProp, "StepSize");
	XMLParser::SetNodeValue(doc, stepSize, m_data.stepSize);
	auto cyclesToSS = XMLParser::AppendNode(doc, electricalProp, "CyclesToSS");
	XMLParser::SetNodeValue(doc, cyclesToSS, m_data.cyclesToSS);
	auto recordFrequency = XMLParser::AppendNode(doc, electricalProp, "RecordFrequency");
	XMLParser::SetNodeValue(doc, recordFrequency, m_data.recordFrequency);
	auto useMedianFilter = XMLParser::AppendNode(doc, electricalProp, "UseMedianFilter");
	XMLParser::SetNodeValue(doc, useMedianFilter, m_data.useMedianFilter);
	auto numMaxHarmonics = XMLParser::AppendNode(doc, electricalProp, "NumMaxHarmonics");
	XMLParser::SetNodeValue(doc, numMaxHarmonics, m_data.numMaxHarmonics);

	return elementNode;
}

bool EMTElement::OpenElement(rapidxml::xml_node<>* elementNode, std::vector<Element*> parentList)
{
	if (!OpenCADProperties(elementNode, parentList)) return false;

	auto electricalProp = elementNode->first_node("ElectricalProperties");
	if (!electricalProp) return false;

	SetOnline(XMLParser::GetNodeValueInt(electricalProp, "IsOnline"));
	m_data.name = electricalProp->first_node("Name")->value();
	m_data.atpFile = wxFileName(electricalProp->first_node("ATPFilePath")->value());
	m_data.atpNodeName = electricalProp->first_node("ATPNodeName")->value();
	m_data.stepSize = XMLParser::GetNodeValueDouble(electricalProp, "StepSize");
	m_data.cyclesToSS = XMLParser::GetNodeValueInt(electricalProp, "CyclesToSS");
	m_data.recordFrequency = XMLParser::GetNodeValueInt(electricalProp, "RecordFrequency");
	m_data.useMedianFilter = XMLParser::GetNodeValueInt(electricalProp, "UseMedianFilter");
	m_data.numMaxHarmonics = XMLParser::GetNodeValueInt(electricalProp, "NumMaxHarmonics");

	m_inserted = true;
	return true;
}

wxArrayString EMTElement::GetATPNodes(wxArrayString atpFile)
{
	wxArrayString nodeList;
	uint32_t mode = 0;
	std::vector<wxString> mode0Cards{ "/OUTPUT", "BLANK", "TACS", "INCLUDE", "TRANSFORMER" };
	std::vector<wxString> mode1Cards{ "/SOURCE" };
	std::vector<wxString> mode2Cards{ "/BRANCH", "/SWITCH", };
	wxString ground = "000000";
	for (wxString line : atpFile) {
		if (line.IsEmpty()) continue;
		if (tolower(line[0]) == 'c') continue; // Skip comments

		bool cardHeader = false;
		for (wxString card : mode0Cards) {
			if (line.Find(card) != wxNOT_FOUND) {
				mode = 0;
				cardHeader = true;
				break;
			}
		}
		for (wxString card : mode1Cards) {
			if (line.Find(card) != wxNOT_FOUND) {
				mode = 1;
				cardHeader = true;
				break;
			}
		}
		for (wxString card : mode2Cards) {
			if (line.Find(card) != wxNOT_FOUND) {
				mode = 2;
				cardHeader = true;
				break;
			}
		}
		if (cardHeader) continue;


		wxString node = "";
		switch (mode)
		{
		case 1: {
			node = line(2, 6).Trim();
			node.Replace(" ", "0");
			if (node != ground && node[5] >= 'A') { // Ignore ground and monophasic nodes
				if (nodeList.Index(node(0, 5)) == wxNOT_FOUND)
					nodeList.Add(node(0, 5));
			}
		} break;
		case 2: {
			node = line(2, 6);
			if (node != ground && node[5] >= 'A') { // Ignore ground and monophasic nodes
				if (nodeList.Index(node(0, 5)) == wxNOT_FOUND)
					nodeList.Add(node(0, 5));
			}
			node = line(8, 6);
			if (node != ground && node[5] >= 'A') { // Ignore ground and monophasic nodes
				if (nodeList.Index(node(0, 5)) == wxNOT_FOUND)
					nodeList.Add(node(0, 5));
			}
		} break;
		}
	}
	return nodeList;
}

bool EMTElement::SetATPParameter(wxTextFile& atpFile, const wxString& card, const int& line, const int& initPos, const int& size, const wxString& value)
{
	bool foundCard = false;
	int currLine = -1;
	wxString lineStr = "";
	lineStr = atpFile.GetFirstLine();
	while (!atpFile.Eof())
	{
		if (tolower(lineStr[0]) == 'c') {
			lineStr = atpFile.GetNextLine();
			continue; // Skip comments
		}
		if (lineStr.Find(card) != wxNOT_FOUND) {
			foundCard = true;
			currLine = -1;
		}
		if (foundCard) currLine++;
		if (foundCard && currLine == line) {
			for (int i = initPos; i < initPos + size; i++) {
				lineStr[i] = value[i - initPos];
			}
			atpFile.RemoveLine(atpFile.GetCurrentLine());
			atpFile.InsertLine(lineStr, atpFile.GetCurrentLine());
			return true;
		}

		lineStr = atpFile.GetNextLine();
	}
	return false;
}

bool EMTElement::AddConnectionToNode(wxTextFile& atpFile, const wxString& node)
{
	wxString switchMask = "  PSPNC%c%s%c                                        MEASURING                %d";
	wxString sourceMask = "14PSPNC%c  %s%s%s                           -1.      100.";
	wxString lineStr = "";
	int outCardPos = -1, switchCardPos = -1, sourceCardPos = -1;
	lineStr = atpFile.GetFirstLine();
	while (!atpFile.Eof())
	{
		if (lineStr.IsEmpty()) {
			lineStr = atpFile.GetNextLine();
			continue;
		}
		if (tolower(lineStr[0]) == 'c') {
			lineStr = atpFile.GetNextLine();
			continue; // Skip comments
		}

		if (lineStr.Find("/SWITCH") != wxNOT_FOUND)
			switchCardPos = atpFile.GetCurrentLine() + 1;

		if (lineStr.Find("/SOURCE") != wxNOT_FOUND)
			sourceCardPos = atpFile.GetCurrentLine() + 1;

		if (lineStr.Find("/OUTPUT") != wxNOT_FOUND)
			outCardPos = atpFile.GetCurrentLine();

		lineStr = atpFile.GetNextLine();
	}

	if (outCardPos < 0) return false;

	if (switchCardPos < 0 && outCardPos > 0) {
		atpFile.InsertLine("/SWITCH", outCardPos);
		switchCardPos = outCardPos + 1;
	}

	if (sourceCardPos < 0 && outCardPos > 0) {
		atpFile.InsertLine("/SOURCE", outCardPos);
		sourceCardPos = outCardPos + 1;
	}

	for (char i = 'A'; i <= 'C'; ++i) {
		lineStr = wxString::Format(switchMask, i, node, i, i == 'A' ? 1 : 0);
		atpFile.InsertLine(lineStr, switchCardPos + (i - 'A'));
	}
	sourceCardPos += 3;

	wxString amp = wxString::FromCDouble(std::abs(m_data.puVoltage) * m_data.baseVoltage, 3); // Amplitude in pu
	wxString freq = wxString::FromCDouble(m_data.frequency, 6); // Frequency in Hz
	// Insert spaces before to complete 10 characters
	while (freq.Length() < 10) freq = " " + freq;
	while (amp.Length() < 10) amp = " " + amp;

	for (char i = 'A'; i <= 'C'; ++i) {
		wxString angle = wxString::FromCDouble(std::arg(m_data.puVoltage) * 180.0 / M_PI - 120.0 * (i - 'A'), 5); // Angle in degrees

		// Insert spaces before to complete 10 characters
		while (angle.Length() < 10) angle = " " + angle;

		lineStr = wxString::Format(sourceMask, i, amp, freq, angle);
		atpFile.InsertLine(lineStr, sourceCardPos + (i - 'A'));
	}

	return true;
}

std::vector<double> EMTElement::MedianFilter(const std::vector<double>& data)
{
	// Based on: http://www.librow.com/articles/article-1

	size_t n = data.size();
	std::vector<double> result(data);

	//   Check arguments
	if (data.empty() || n < 1)
		return result;
	//   Treat special case N = 1
	if (n == 1)
	{
		result[0] = data[0];
		return result;
	}
	//   Allocate memory for signal extension
	double* extension = new double[n + 4];
	//   Check memory allocation
	//if (!extension)
	//	return;
	//   Create signal extension
	memcpy(extension + 2, data.data(), n * sizeof(double));
	for (int i = 0; i < 2; ++i)
	{
		extension[i] = data[1 - i];
		extension[n + 2 + i] = data[n - 1 - i];
	}
	//   Call median filter implementation
	result = DoMedianFilter(extension, result, n + 4);
	//   Free memory
	delete[] extension;

	return result;
}

bool EMTElement::CalculateCurrent(const PropertiesData& properties, wxString& errorMsg, const bool& saveFFTData)
{
	wxFileName fileName(m_data.atpFile);
	if (!fileName.IsOk()) {
		errorMsg = wxString::Format(_("Invalid ATP file path for the electromagnetic element \"%s\".", m_data.name));
		return false;
	}

	double fundFreq = properties.GetSimulationPropertiesData().stabilityFrequency;
	double timeSim = (1.0 / fundFreq) * static_cast<double>(m_data.cyclesToSS);

	wxString atpFolder = properties.GetGeneralPropertiesData().atpPath.GetPath();
	// Set fundamental frequency to EMT element
	m_data.frequency = fundFreq;

	wxExecuteEnv env;
	env.cwd = atpFolder;

	// Save the ATP file in ATP work folder
	wxTextFile origFile(m_data.atpFile.GetFullPath());
	fileName.SetPath(properties.GetGeneralPropertiesData().atpWorkFolder);
	wxString name = fileName.GetFullPath();
	wxTextFile copyFile(fileName.GetFullPath());
	if (origFile.Open()) {
		copyFile.Create();
		for (size_t i = 0; i < origFile.GetLineCount(); i++) {
			copyFile.AddLine(origFile.GetLine(i));
		}
		origFile.Close();

		wxString stepSizeStr = wxString::Format("%.2E", m_data.stepSize);
		stepSizeStr.Replace(wxT(","), wxT("."));
		wxString timeStr = wxString::Format("%.6f", timeSim);
		timeStr.Replace(wxT(","), wxT("."));
		SetATPParameter(copyFile, wxT("BEGIN NEW DATA CASE"), 1, 0, 8, stepSizeStr);
		SetATPParameter(copyFile, wxT("BEGIN NEW DATA CASE"), 1, 8, 8, timeStr);
		SetATPParameter(copyFile, wxT("BEGIN NEW DATA CASE"), 2, 0, 8, wxT("99999999"));
		SetATPParameter(copyFile, wxT("BEGIN NEW DATA CASE"), 2, 8, 8, wxString::Format("%8d", m_data.recordFrequency));

		// Add source and switch to selectec node
		AddConnectionToNode(copyFile, m_data.atpNodeName);

		copyFile.Write();
	}
	else {
		errorMsg = wxString::Format(_("Fail to open ATP file of the electromagnetic element \"%s\".", m_data.name));
		return false;
	}

	wxString cmd = properties.GetGeneralPropertiesData().atpPath.GetFullPath() + wxT(" both ") + fileName.GetFullPath() + wxT(" s -R");
	wxExecute(cmd, wxEXEC_SYNC | wxEXEC_HIDE_CONSOLE, nullptr, &env);

	// Delete all .tmp files
	wxDir dir(atpFolder);
	if (dir.IsOpened()) {
		wxString file;
		bool cont = dir.GetFirst(&file, wxT("*.tmp"), wxDIR_FILES);
		while (cont) {
			wxRemoveFile(atpFolder + wxT("/") + file);
			cont = dir.GetNext(&file);
		}
	}

	fileName.SetFullName(fileName.GetName() + wxT(".pl4"));
	wxFileInputStream pl4File(fileName.GetFullPath());
	// https://github.com/ldemattos/readPL4/wiki/PISA's-PL4-Format---Time-domain
	if (pl4File.IsOk() && pl4File.GetSize() > 0) {
		wxDataInputStream store(pl4File);
		store.UseBasicPrecisions();
		float* buffer = new float[2];
		std::vector<double> timeVec;
		std::vector<double> valueVec;
		double oldTime = 0.0;
		int read16Bytes = 0;
		while (!pl4File.Eof()) {
			store.ReadFloat(buffer, 2);
			if (read16Bytes > 11 && (buffer[0] >= oldTime)) {
				oldTime = buffer[0];
				timeVec.emplace_back(buffer[0]);
				valueVec.emplace_back(buffer[1]);
			}
			read16Bytes++;
		}
		delete[] buffer;

		if (m_data.useMedianFilter)
			valueVec = MedianFilter(valueVec);

		// FFT
		double dataStepSize = timeVec[1] - timeVec[0]; // Real sampling time from data
		bool useRemainder = false;
		size_t n = ceil(1.0 / (dataStepSize * fundFreq)); // Number of samples
		// Due to ATP logic, the number of samples can be greater than the number of samples in the PL4 file
		// In this case, I choose to follow the remainder approach
		if (n > valueVec.size()) {
			n = valueVec.size();
			useRemainder = true;
		}

		double fs = 1.0 / dataStepSize; // Sampling frequency
		double df = fs / static_cast<double>(n); // Frequency resolution

		if (useRemainder) {
			double rmder = abs(remainder(fundFreq, df));
			int minRmderN = n;
			while (rmder > 1e-3) {
				n--;
				df = fs / static_cast<double>(n);

				if (abs(remainder(fundFreq, df)) < rmder) minRmderN = n;

				rmder = abs(remainder(fundFreq, df));

				if (n <= 0) {
					n = minRmderN;
					df = fs / static_cast<double>(n);
					break;
				}
			}
		}

		double dtWindow = timeVec[timeVec.size() - 1] - timeVec[timeVec.size() - n];

		fftw_complex* out;
		double* in;
		fftw_plan p;
		out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (n / 2 + 1));
		in = (double*)fftw_malloc(sizeof(double) * n);
		// Populate input array
		for (size_t i = (valueVec.size() - n); i < valueVec.size(); ++i) {
			size_t index = i - (valueVec.size() - n);
			in[index] = valueVec[i];
		}

		p = fftw_plan_dft_r2c_1d(n, in, out, FFTW_ESTIMATE);
		fftw_execute(p); /* repeat as needed */
		fftw_destroy_plan(p);

		double ampCorrection = 2.0 / static_cast<double>(n);

		// Identify fundamental frequency
		int fundIndex = -1;
		double fundMagnitude = 0.0;
		for (size_t i = 0; i < (n / 2 + 1); i++) {
			double freq = static_cast<double>(i) * df;
			if (remainder(freq, fundFreq)) {
				fundIndex = i;
				fundMagnitude = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]) * ampCorrection;
				break;
			}
		}
		if (fundIndex < 0) {
			errorMsg = wxString::Format(_("Fail to identify the fundamental frequency of the electromagnetic element \"%s\"."), m_data.name);
			return false;
		}

		// Fundamental and harmonics
		m_data.currHarmonics.clear();
		m_data.currHarmonicsOrder.clear();
		for (size_t i = 0; i < (n / 2 + 1); i++) {
			double freq = static_cast<double>(i) * df;
			int order = static_cast<int>(round(freq / fundFreq));

			if (order > m_data.numMaxHarmonics) break; // Stop if the order is greater than the maximum number of harmonics

			double magnitude = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]) * ampCorrection;
			if ((magnitude / fundMagnitude) > (m_data.harmonicsThreshold / 100.0)) {
				m_data.currHarmonics.emplace_back(std::complex<double>(out[i][0], out[i][1]) * ampCorrection);
				m_data.currHarmonicsOrder.emplace_back(order);
			}
		}

		if (saveFFTData) {
			m_data.inFFTData.clear();
			m_data.outFFTData.clear();
			for (size_t i = 0; i < n; i++) {
				m_data.inFFTData.emplace_back(std::make_pair(timeVec[i + timeVec.size() - n], in[i]));
			}
			for (size_t i = 0; i < (n / 2 + 1); i++) {
				std::complex<double> value(out[i][0] * ampCorrection, out[i][1] * ampCorrection);
				double freq = static_cast<double>(i) * df;
				m_data.outFFTData.emplace_back(std::make_pair(freq, value));
			}
		}

		fftw_free(in);
		fftw_free(out);

	}
	else {
		wxString atpErrorMsg = _("No error found.");

		fileName.SetFullName(fileName.GetName() + wxT(".lis"));
		wxTextFile lisFile(fileName.GetFullPath());

		if (!lisFile.Exists()) {
			errorMsg = wxString::Format(_("Fail to run ATP file of the eletromagnetic element \"%s\".\nThe ATP program doesn't return any error message."), m_data.name);
			return false;
		}
		if (lisFile.Open()) {
			wxString line = lisFile.GetFirstLine() + "\n";
			bool foundError = false;
			int lineCount = -1;
			atpErrorMsg.Clear();
			while (!lisFile.Eof())
			{
				if (line.Find(wxT("KILL ")) != wxNOT_FOUND) {
					foundError = true;
					lineCount++;
				}
				if (line.Find(wxT("----------")) != wxNOT_FOUND) {
					foundError = false;
				}
				if (foundError && lineCount == 1)
					atpErrorMsg += line + " ";
				line = lisFile.GetNextLine();
			}
			lisFile.Close();
		}
		else
		{
			errorMsg = wxString::Format(_("Fail to run ATP file of the electromagnetic element \"%s\".\nThe ATP program doesn't return any error message."), m_data.name);
			return false;
		}

		errorMsg = wxString::Format(_("Fail to run ATP file of the electromagnetic element \"%s\".\nThe ATP return the following error message:\n\"%s\""), m_data.name, atpErrorMsg);
		return false;
	}
	return true;
}

std::vector<double> EMTElement::DoMedianFilter(double* extension, std::vector<double>& result, const int& n)
{
	//   Move window through all elements of the signal
	for (int i = 2; i < n - 2; ++i)
	{
		//   Pick up window elements
		double window[5];
		for (int j = 0; j < 5; ++j)
			window[j] = extension[i - 2 + j];
		//   Order elements (only half of them)
		for (int j = 0; j < 3; ++j)
		{
			//   Find position of minimum element
			int min = j;
			for (int k = j + 1; k < 5; ++k)
				if (window[k] < window[min])
					min = k;
			//   Put found minimum element in its place
			const double temp = window[j];
			window[j] = window[min];
			window[min] = temp;
		}
		//   Get result - the middle element
		result[i - 2] = window[2];
	}
	return result;
}
