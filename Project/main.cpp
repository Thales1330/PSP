#include <wx/app.h>
#include <wx/event.h>
#include <wx/image.h>
#include <wx/stdpaths.h>
#include <wx/textfile.h>

#include "MainFrame.h"
#include "PropertiesData.h"

// Define the MainApp
class MainApp : public wxApp
{
   public:
    MainApp() {}
    virtual ~MainApp() {}
    void LoadInitFile(PropertiesData* propertiesData)
    {
        wxTextFile file("config.ini");
        auto data = propertiesData->GetGeneralPropertiesData();

        if(!file.Open()) {
            // Create default init file.
            wxString defaultInitFile = "lang=en\ntheme=light\n";

            file.Create();
            file.AddLine(defaultInitFile);
            file.Write();
            file.Close();

            data.language = wxLANGUAGE_ENGLISH;
            data.theme = THEME_LIGHT;
            propertiesData->SetGeneralPropertiesData(data);
        }

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
        }
        file.Close();

        propertiesData->SetGeneralPropertiesData(data);
    }

    void LoadCatalogs(wxLocale* locale, PropertiesData* propertiesData)
    {
        LoadInitFile(propertiesData);

        // Load computer settings.
        locale->Init(locale->GetSystemLanguage(), wxLOCALE_DONT_LOAD_DEFAULT);

        wxFileName fn(wxStandardPaths::Get().GetExecutablePath());
        wxString langPath = fn.GetPath() + "\\..\\data\\lang";
        locale->AddCatalogLookupPathPrefix(langPath);
        // Load translation catalogs.
        locale->AddCatalog(wxT("pt_BR"), wxLANGUAGE_PORTUGUESE_BRAZILIAN);
    }

    virtual bool OnInit()
    {
        // Add image handlers
        wxImage::AddHandler(new wxPNGHandler);
        wxImage::AddHandler(new wxJPEGHandler);

        PropertiesData* propertiesData = new PropertiesData();

        wxLocale* locale = new wxLocale();
        LoadCatalogs(locale, propertiesData);

        MainFrame* mainFrame = new MainFrame(NULL, locale, propertiesData);
        SetTopWindow(mainFrame);
        return GetTopWindow()->Show();
    }
};

DECLARE_APP(MainApp)
IMPLEMENT_APP(MainApp)
