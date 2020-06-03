#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/event.h>
#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>
#include <wx/textfile.h>

#include <wx/cmdline.h>
#include <wx/msgout.h> 

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "MainFrame.h"
#include "PropertiesData.h"

/*static const wxCmdLineEntryDesc g_cmdLineDesc[] = {
    {wxCMD_LINE_SWITCH, "h", "help", "displays help on the command line parameters", wxCMD_LINE_VAL_NONE,
     wxCMD_LINE_OPTION_HELP},
    {wxCMD_LINE_SWITCH, "t", "test", "test switch", wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_MANDATORY},
    {wxCMD_LINE_SWITCH, "s", "silent", "disables the GUI"},

    {wxCMD_LINE_NONE}};*/

// Define the MainApp
class MainApp : public wxApp
{
   public:
    MainApp() {}
    virtual ~MainApp() {}
    bool LoadInitFile(PropertiesData* propertiesData)
    {
        // Load configuration file, if don't exists create it.
        // Find the executable location path.
        wxFileName fn(wxStandardPaths::Get().GetExecutablePath());
        wxTextFile file(fn.GetPath() + wxFileName::GetPathSeparator() + "config.ini");
        auto data = propertiesData->GetGeneralPropertiesData();

        if(!file.Create()) {
            if(!file.Open()) return false;

            wxString line;
            for(line = file.GetFirstLine(); !file.Eof(); line = file.GetNextLine()) {
                wxString tag = "";
                wxString tagValue = "";
                bool parseValue = false;
                for(unsigned int i = 0; i < line.Len(); ++i) {
                    if(line[i] == '=') {
                        parseValue = true;
                    } else {
                        if(parseValue)
                            tagValue += line[i];
                        else
                            tag += line[i];
                    }
                }
                // Language
                if(tag == "lang") {
                    if(tagValue == "pt-br" || tagValue == "pt") {
                        data.language = wxLANGUAGE_PORTUGUESE_BRAZILIAN;
                    } else if(tagValue == "en" || tagValue == "en-us" || tagValue == "en-uk") {
                        data.language = wxLANGUAGE_ENGLISH;
                    }
                }
                if(tag == "theme") {
                    if(tagValue == "light") {
                        data.theme = THEME_LIGHT;
                    } else if(tagValue == "dark") {
                        data.theme = THEME_DARK;
                    }
                }
				if (tag == "useOpenGL") {
					if (tagValue == "yes") {
						data.useOpenGL = true;
					}
					else if (tagValue == "no") {
						data.useOpenGL = false;
					}
				}
            }
            file.Close();
        } else {  // Create default init file.
            if(!file.Open()) return false;

            // Default parameters.
            file.AddLine("lang=en");
            file.AddLine("theme=light");
            file.AddLine("useOpenGL=yes");

            file.Write();
            file.Close();

            data.language = wxLANGUAGE_ENGLISH;
            data.theme = THEME_LIGHT;
            data.useOpenGL = true;
            propertiesData->SetGeneralPropertiesData(data);
        }

        propertiesData->SetGeneralPropertiesData(data);
        return true;
    }

    void LoadCatalogs(wxLocale* locale, PropertiesData* propertiesData)
    {
        // Load language catalogs according the propertiesData attribute.
        if(!locale->Init(propertiesData->GetGeneralPropertiesData().language, wxLOCALE_DONT_LOAD_DEFAULT)) {
            wxMessageDialog msgDialog(NULL, _("This language is not supported by the system."), _("Error"),
                                      wxOK | wxCENTRE | wxICON_ERROR);
            msgDialog.ShowModal();
        }

        wxFileName fn(wxStandardPaths::Get().GetExecutablePath());
        wxString langPath = fn.GetPath() + wxFileName::DirName("\\..\\data\\lang", wxPATH_WIN).GetPath();
        locale->AddCatalogLookupPathPrefix(langPath);
        // pt_BR
        if(propertiesData->GetGeneralPropertiesData().language == wxLANGUAGE_PORTUGUESE_BRAZILIAN) {
            if(!locale->AddCatalog(wxT("pt_BR"))) {
                wxMessageDialog msgDialog(NULL, _("Fail to load brazilian portuguese language catalog."), _("Error"),
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
        if(cmdLineParser.Parse() == 0) {
            wxCmdLineArgs args = cmdLineParser.GetArguments();
            for(auto it = args.begin(), itEnd = args.end(); it != itEnd; ++it) {
                if(it->GetKind() == wxCMD_LINE_PARAM) { openFilePath = it->GetStrVal();}
                else if(it->GetShortName() == "t") {
                    test = true;
                }
            }
        }
        MainFrame* mainFrame = new MainFrame(NULL, locale, propertiesData, openFilePath);
#ifdef __WXMSW__
        // Set application icon for windows
        mainFrame->SetIcon(wxICON(aaaaprogicon));
#endif
        SetTopWindow(mainFrame);
        if(test) {
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
