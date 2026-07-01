/*
 *  Copyright (C) 2026  Thales Lima Oliveira <thales.oliveira@uftm.edu.br>
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

#ifndef CONTROLEDITORMANAGER_H
#define CONTROLEDITORMANAGER_H

#include <vector>

class Workspace;
class ControlEditor;
class ControlElementContainer;

/**
 * @class ControlEditorManager
 * @author Thales Lima Oliveira <thales.oliveira@uftm.edu.br>
 * @date 30/06/2026
 * @brief This class is responsible to manage the ControlEditor instances in the application.
 * @file ControlEditorManager.h
 */
class ControlEditorManager
{
public:
    explicit ControlEditorManager(Workspace* workspace);
    ~ControlEditorManager();

    ControlEditor* Open(ControlElementContainer* container, int plotLib, int ioFlags);

    void Remove(ControlEditor* editor);

    void CloseAll();

    bool IsOpen(ControlElementContainer* container) const;

    ControlEditor* GetEditor(const ControlElementContainer* container) const;

private:
    Workspace* m_workspace = nullptr;

    std::vector<ControlEditor*> m_editors;
};

#endif