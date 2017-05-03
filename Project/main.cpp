#include <wx/app.h>
#include <wx/event.h>
#include <wx/image.h>
#include <wx/stdpaths.h>

#include "MainFrame.h"

// Define the MainApp
class MainApp : public wxApp
{
public:
    MainApp() {}
    virtual ~MainApp() {}
    void LoadCatalogs(wxLocale* locale)
    {
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

        wxLocale* locale = new wxLocale();
        LoadCatalogs(locale);

        MainFrame* mainFrame = new MainFrame(NULL, locale);
        SetTopWindow(mainFrame);
        return GetTopWindow()->Show();
    }
};

DECLARE_APP(MainApp)
IMPLEMENT_APP(MainApp)
