#ifdef __INTELLISENSE__
#pragma diag_suppress 102
#endif

#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/event.h>
#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>
#include <wx/textfile.h>

#include <wx/cmdline.h>
#include <wx/msgout.h> 

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "MainFrame.h"
#include "utils/PropertiesData.h"

/** @file main.cpp */
/** @mainpage PSP-UFU
 * [**PSP-UFU website**](https://thales1330.github.io/PSP/)
 *
 *PSP-UFU (Power Systems Platform of Federal University of Uberlândia) is a **cross-platform**, **multilingual**, **Free and Open-Source Software** (FOSS) with **advanced GUI** (Graphical User Interface) features and **CAD** (Computer-Aided Design) tools for power system studies.
 *
 *The software allows for the construction of any electric **transmission network** and **control systems** through the deployment of visual elements.
 *
 *For the visualization of results, the program offers linked text elements in the main screen, and also table and graph editors.
 *
 *The PSP-UFU  aims to provide efficient computer simulation tools for **research and education purposes**, in addition to **industrial applications** in electrical power systems.
 *
 *The software can perform the following studies:
 *
 *- **Power Flow**
 *  - Newton-Raphson
 *  - Gauss-Seidel
 *  - Hybrid Newton-Gauss
 *  - Three-phase induction motors included in power flow studies
 *- **Short-Circuit calculation**
 *  - Balanced
 *  - Unbalanced
 *  - Short-Circuit power in all system buses
 *- **Harmonics**
 *  - Harmonic voltages and THD (Total Harmonic Distortion) calculation
 *  - Frequency scan
 *- **Transient and Dynamic Stability**
 *  - Several synchronous machine models automatically selected
 *  - Three-phase induction motors
 *  - User-defined machine controls, exciters and prime moves created using block diagrams (Exciters, AVR, PSS, Hydro and Thermal turbines, Speed Governor, etc.)
 *
 *@section Published Papers
 *Further details can be found in the published papers:
 *
 *> [1] Oliveira, T. L., Guimarães, G. C., & Silva, L. R. C. (2019). PSP-UFU: An open-source, graphical, and multiplatform software for power system studies. _International Transactions on Electrical Energy Systems_, e12185. doi: [10.1002/2050-7038.12185](https://doi.org/10.1002/2050-7038.12185)<br>
 *> [2] Oliveira, T. L., Guimarães, G. C., Silva, L. R., & Rezende, J. O. (2019). Power system education and research applications using free and open-source, graphical and multiplatform PSP-UFU software. _The International Journal of Electrical Engineering & Education_, 0020720919879058. doi: [10.1177/0020720919879058](https://doi.org/10.1177/0020720919879058).
 *
 * @section author Author and License
 *  PSP-UFU is published under the terms of the GPL-v2 license.<br>
 *  The author of this project is Thales Lima Oliveira <thales@ufu.br>.<br>
 *
 *	The project is hosted on GitHub: [PSP-UFU Repository](https://github.com/Thales1330/PSP)
 */

 // Define the MainApp
class MainApp : public wxApp
{
public:

	MainApp() {}
	virtual ~MainApp() {}
	bool LoadInitFile(PropertiesData* propertiesData)
	{
#ifdef _DEBUG
		wxSizerFlags::DisableConsistencyChecks();
#endif

		// Load configuration file, if don't exists create it.
		wxFileName fn(wxStandardPaths::Get().GetDocumentsDir() + wxFileName::GetPathSeparator() + "PSP-UFU" + wxFileName::GetPathSeparator() + "config.ini");
		if (!fn.DirExists()) {
			fn.Mkdir();
		}

		wxTextFile file(fn.GetFullPath());
		auto data = propertiesData->GetGeneralPropertiesData();

		if (!file.Create()) {
			if (!file.Open()) return false;

			wxString line;
			for (line = file.GetFirstLine(); !file.Eof(); line = file.GetNextLine()) {
				wxString tag = "";
				wxString tagValue = "";
				bool parseValue = false;
				for (unsigned int i = 0; i < line.Len(); ++i) {
					if (line[i] == '=') {
						parseValue = true;
					}
					else {
						if (parseValue)
							tagValue += line[i];
						else
							tag += line[i];
					}
				}
				// Language
				if (tag == "lang") {
					if (tagValue == "pt-br" || tagValue == "pt") {
						data.language = wxLANGUAGE_PORTUGUESE_BRAZILIAN;
					}
					else if (tagValue == "en" || tagValue == "en-us" || tagValue == "en-uk") {
						data.language = wxLANGUAGE_ENGLISH;
					}
				}
				if (tag == "theme") {
					if (tagValue == "light") {
						data.theme = THEME_LIGHT;
					}
					else if (tagValue == "dark") {
						data.theme = THEME_DARK;
					}
				}
				if (tag == "plotlib") {
					if (tagValue == "chartdir") {
						data.plotLib = PlotLib::wxCHART_DIR;
					}
					else if (tagValue == "mathplot") {
						data.plotLib = PlotLib::wxMATH_PLOT;
					}
				}
				//if (tag == "useOpenGL") {
				//	if (tagValue == "yes") {
				//		data.useOpenGL = true;
				//	}
				//	else if (tagValue == "no") {
				//		data.useOpenGL = false;
				//	}
				//}
				if (tag == "atpfile") {
					data.atpPath = wxFileName(tagValue);
				}
			}
			file.Close();
		}
		else {  // Create default init file.
			if (!file.Open()) return false;

			// Default parameters.
			file.AddLine("lang=en");
			file.AddLine("plotlib=chartdir");
			file.AddLine("theme=light");
			file.AddLine("atpfile=");
			//file.AddLine("useOpenGL=yes");

			file.Write();
			file.Close();

			data.language = wxLANGUAGE_ENGLISH;
			data.theme = THEME_LIGHT;
			data.plotLib = PlotLib::wxCHART_DIR;
			//data.useOpenGL = true;
			propertiesData->SetGeneralPropertiesData(data);
		}

		// Check if the atp folder exists, if not create it.
		wxString atpFolder = wxStandardPaths::Get().GetDocumentsDir() + wxFileName::GetPathSeparator() + "PSP-UFU" + wxFileName::GetPathSeparator() + "atp";
		if (!wxDir::Exists(atpFolder)) wxDir::Make(atpFolder);
		data.atpWorkFolder = atpFolder;

		propertiesData->SetGeneralPropertiesData(data);
		return true;
	}

	void LoadCatalogs(wxLocale* locale, PropertiesData* propertiesData)
	{
		// Load language catalogs according the propertiesData attribute.
		if (!locale->Init(propertiesData->GetGeneralPropertiesData().language, wxLOCALE_DONT_LOAD_DEFAULT)) {
			wxMessageDialog msgDialog(nullptr, _("This language is not supported by the system."), _("Error"),
				wxOK | wxCENTRE | wxICON_ERROR);
			msgDialog.ShowModal();
		}

		wxFileName fn(wxStandardPaths::Get().GetExecutablePath());
		wxString langPath = fn.GetPath() + wxFileName::DirName("\\..\\data\\lang", wxPATH_WIN).GetPath();
		locale->AddCatalogLookupPathPrefix(langPath);
		// pt_BR
		if (propertiesData->GetGeneralPropertiesData().language == wxLANGUAGE_PORTUGUESE_BRAZILIAN) {
			if (!locale->AddCatalog(wxT("pt_BR"))) {
				wxMessageDialog msgDialog(nullptr, _("Fail to load brazilian portuguese language catalog."), _("Error"),
					wxOK | wxCENTRE | wxICON_ERROR);
				msgDialog.ShowModal();
			}
		}
	}

	virtual bool OnInit()
	{
		// Add image handlers
		wxImage::AddHandler(new wxPNGHandler);
		wxImage::AddHandler(new wxJPEGHandler);

		// Load fonts
#ifdef wxUSE_PRIVATE_FONTS
		wxFileName fn(wxStandardPaths::Get().GetExecutablePath());
		wxString fontsPath = fn.GetPath() + wxFileName::DirName("\\..\\data\\fonts", wxPATH_WIN).GetPath();
		bool loadFont = wxFont::AddPrivateFont(fontsPath + wxFileName::GetPathSeparator() + "cmunrm.ttf");
		if (loadFont) loadFont = wxFont::AddPrivateFont(fontsPath + wxFileName::GetPathSeparator() + "cmunbx.ttf");
		if (!loadFont) {
			wxMessageDialog msgDialog(nullptr, _("Fail to load local font."), _("Error"),
				wxOK | wxCENTRE | wxICON_ERROR);
			msgDialog.ShowModal();
		}
#endif // wxUSE_PRIVATE_FONTS		

		PropertiesData* propertiesData = new PropertiesData();
		LoadInitFile(propertiesData);

		wxLocale* locale = new wxLocale();
		LoadCatalogs(locale, propertiesData);

		wxString openFilePath = "";

		// Load command line attributes. This is used to directly open saved files (.psp).
		wxCmdLineParser cmdLineParser(wxApp::argc, wxApp::argv);
		//cmdLineParser.SetDesc(cmdLineDesc);
		cmdLineParser.AddParam("", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
		cmdLineParser.AddSwitch("t", "test", "Run PSP-UFU tests");

		bool test = false;
		if (cmdLineParser.Parse() == 0) {
			wxCmdLineArgs args = cmdLineParser.GetArguments();
			for (auto it = args.begin(), itEnd = args.end(); it != itEnd; ++it) {
				if (it->GetKind() == wxCMD_LINE_PARAM) { openFilePath = it->GetStrVal(); }
				else if (it->GetShortName() == "t") {
					test = true;
				}
			}
		}
		MainFrame* mainFrame = new MainFrame(nullptr, locale, propertiesData, openFilePath);
#ifdef __WXMSW__
		// Set application icon for windows
		mainFrame->SetIcon(wxICON(aaaaprogicon));
#endif
		SetTopWindow(mainFrame);
		if (test) {
			GetTopWindow()->Show();
			exit(mainFrame->RunPSPTest());
		}
		return GetTopWindow()->Show();
	}

	virtual int OnExit()
	{
		// clean up
		return 0;
	}

	/*virtual int OnRun()
	{
		int exitcode = wxApp::OnRun();
		// wxTheClipboard->Flush();
		if(exitcode != 0) return exitcode;
	}

	virtual void OnInitCmdLine(wxCmdLineParser& parser)
	{
		parser.SetDesc(g_cmdLineDesc);
		// must refuse '/' as parameter starter or cannot use "/path" style paths
		parser.SetSwitchChars(wxT("-"));
	}

	virtual bool OnCmdLineParsed(wxCmdLineParser& parser)
	{
		//silent_mode = parser.Found(wxT("s"));

		// to get at your unnamed parameters use
		wxArrayString files;
		for(unsigned int i = 0; i < parser.GetParamCount(); i++) { files.Add(parser.GetParam(i));
wxMessageBox(parser.GetParam(i)); }

		// and other command line parameters

		// then do what you need with them.

		return true;
	}*/
};

DECLARE_APP(MainApp)
IMPLEMENT_APP(MainApp)
