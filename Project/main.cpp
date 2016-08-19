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
		locale->Init(
			locale->GetSystemLanguage(),
			wxLOCALE_DONT_LOAD_DEFAULT);  // captura as propriedades locais do computador (idioma, numeração, etc...)

		wxString langPath = wxStandardPaths::Get().GetExecutablePath();
		// remove o nome do executável
		for(int i = langPath.size(); i >= 0; i--) {
			if(langPath[i] == '/' || langPath[i] == '\\') {
				langPath.Truncate(i + 1);
				break;
				}
			}
		langPath += wxT("data\\lang");

		locale->AddCatalogLookupPathPrefix(langPath);
		// Carregar catálogos de tradução
		locale->AddCatalog(wxT("pt_BR"), wxLANGUAGE_PORTUGUESE_BRAZILIAN);
    }

    virtual bool OnInit()
    {
		// Add the common image handlers
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
