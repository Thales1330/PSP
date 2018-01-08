/*
 *  Copyright (C) 2017  Thales Lima Oliveira <thales@ufu.br>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef FILEHANDING_H
#define FILEHANDING_H

#include <wx/string.h>
#include <fstream>
#include <sstream>

#include "Workspace.h"
#include "ControlEditor.h"
#include "ControlElementContainer.h"
#include "ElectricCalculation.h"
#include "Text.h"

#include "XMLParser.h"

/**
 * @class FileHanding
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Save and opens the projects created on disk.
 * @file FileHanding.h
 */
class FileHanding
{
public:
    FileHanding();
    FileHanding(Workspace* workspace);
    FileHanding(ControlEditor* controlEditor);
    ~FileHanding();

    void SetWorkspace(Workspace* workspace) { m_workspace = workspace; }
    void SetControlEditor(ControlEditor* controlEditor) { m_controlEditor = controlEditor; }

    void SaveProject(wxFileName path);
    bool OpenProject(wxFileName path);
    
    void SaveControl(wxFileName path);
    bool OpenControl(wxFileName path, std::vector<ControlElement*>& ctrlElementList, std::vector<ConnectionLine*>& ctrlConnectionList);
    
    void SaveControlElements(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementsNode, ControlElementContainer* ctrlContainer = NULL);
    bool OpenControlElements(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementsNode, ControlElementContainer* ctrlContainer = NULL);

protected:
    Workspace* m_workspace = NULL;
    ControlEditor* m_controlEditor = NULL;
};

#endif // FILEHANDING_H
