#ifndef CONTROLEDITORMANAGER_H
#define CONTROLEDITORMANAGER_H

#include <vector>

class Workspace;
class ControlEditor;
class ControlElementContainer;

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