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

#include "rapidXML/rapidxml.hpp"
// Modified: http://stackoverflow.com/questions/14113923/rapidxml-print-header-has-undefined-methods
#include "rapidXML/rapidxml_print.hpp"
#include "rapidXML/rapidxml_utils.hpp"

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
    bool OpenControl(wxFileName path);

protected:
    Workspace* m_workspace = NULL;
    ControlEditor* m_controlEditor = NULL;

    rapidxml::xml_node<>* AppendNode(rapidxml::xml_document<>& doc,
        rapidxml::xml_node<>* parentNode,
        const char* name,
        rapidxml::node_type nodeType = rapidxml::node_element);
    void SetNodeValue(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, wxString value);
    void SetNodeValue(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, int value);
    void SetNodeValue(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, double value);
    void SetNodeAttribute(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, const char* atrName, wxString value);
    void SetNodeAttribute(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, const char* atrName, int value);
    void SetNodeAttribute(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, const char* atrName, double value);
    double GetNodeValueDouble(rapidxml::xml_node<>* parent, const char* nodeName);
    int GetNodeValueInt(rapidxml::xml_node<>* parent, const char* nodeName);
    int GetAttributeValueInt(rapidxml::xml_node<>* parent, const char* nodeName, const char* atrName);
    int GetAttributeValueInt(rapidxml::xml_node<>* node, const char* atrName);
    
    void SaveControlElements(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementsNode);
    void SaveControlNodes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* nodesN, std::vector<Node*> nodeList);
    //void SaveControlChildren(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* childrenNode, std::vector<Node*> childList);
    ControlElement* GetControlElementFromID(std::vector<ControlElement*> elementList, int id);
};

#endif // FILEHANDING_H
