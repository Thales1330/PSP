/*
 *  Copyright (C) 2026  Thales Lima Oliveira <thales@ufu.br>
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

#include "Path.h"

#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/msgdlg.h>

wxString Paths::GetExecutablePath()
{
	return wxStandardPaths::Get().GetExecutablePath();
}

wxString Paths::GetExecutableDir()
{
	wxFileName exe(wxStandardPaths::Get().GetExecutablePath());
	return exe.GetPath();
}

wxString Paths::GetDataPath()
{
    wxFileName exe(wxStandardPaths::Get().GetExecutablePath());

    // 1. Installed layout (bin/../share/psp-ufu/data)
    wxString installData = exe.GetPath() + "/../share/psp-ufu/data";

    wxFileName fnInstall(installData);
    fnInstall.Normalize(wxPATH_NORM_DOTS);

    if (wxDirExists(fnInstall.GetFullPath()))
        return fnInstall.GetFullPath();

    // 2. macOS / AppImage resources
    wxString resources = wxStandardPaths::Get().GetResourcesDir() + "/data";

    if (wxDirExists(resources))
        return resources;

    // 3. Standard Linux install (/usr/share/psp-ufu/data)
    wxString dataDir = wxStandardPaths::Get().GetDataDir() + "/data";

    if (wxDirExists(dataDir))
        return dataDir;

    // 4. Portable layout (Windows or development build)
    wxString portable = exe.GetPath() + "/../data";

    wxFileName fn(portable);
    fn.Normalize(wxPATH_NORM_DOTS);

    if (wxDirExists(fn.GetFullPath()))
        return fn.GetFullPath();

    wxMessageDialog msgDialog(nullptr,
        _("Data directory not found."),
        _("Error"),
        wxOK | wxCENTRE | wxICON_ERROR);

    msgDialog.ShowModal();

    return "";
}

wxString Paths::GetDocumentsPath()
{
	return wxStandardPaths::Get().GetDocumentsDir();
}
