#include "ControlEditorManager.h"

#include <algorithm>

#include "ControlEditor.h"
#include "Workspace.h"
#include "../elements/controlElement/ControlElementContainer.h"

ControlEditorManager::ControlEditorManager(Workspace* workspace)
    : m_workspace(workspace)
{
}

ControlEditorManager::~ControlEditorManager()
{
    CloseAll();
}

ControlEditor* ControlEditorManager::Open(ControlElementContainer* container, int plotLib, int ioFlags)
{
    ControlEditor* editor = GetEditor(container);

    if (editor)
    {
        //editor->Raise();
        //editor->SetFocus();
        //return editor;
        editor->Close();
        editor = nullptr;
    }

    editor = new ControlEditor(m_workspace, ioFlags);

    editor->SetManager(this);
    editor->SetElementsList(container->GetControlElementsList());
    editor->SetConnectionsList(container->GetConnectionLineList());
    editor->SetControlContainer(container);
    editor->SetPlotLib(plotLib);

    m_editors.push_back(editor);

    editor->Show();
    editor->SetJustOpened(true);

    return editor;
}

void ControlEditorManager::Remove(ControlEditor* editor)
{
    auto it = std::find(m_editors.begin(), m_editors.end(), editor);

    if (it != m_editors.end())
        m_editors.erase(it);
}

void ControlEditorManager::CloseAll()
{
    auto editors = m_editors;

    for (auto* editor : editors)
    {
        if (editor)
            editor->Destroy();
    }

    m_editors.clear();
}

bool ControlEditorManager::IsOpen(ControlElementContainer* container) const
{
    return GetEditor(container) != nullptr;
}

ControlEditor* ControlEditorManager::GetEditor(const ControlElementContainer* container) const
{
    for (auto* editor : m_editors)
    {
        if (!editor)
            continue;

        if (editor->GetControlContainer() == container)
            return editor;
    }

    return nullptr;
}