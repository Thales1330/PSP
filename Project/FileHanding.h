#ifndef FILEHANDING_H
#define FILEHANDING_H

#include <wx/string.h>
#include <fstream>
#include <sstream>

#include "Workspace.h"
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
    ~FileHanding();

    void SetWorkspace(Workspace* workspace) { m_workspace = workspace; }

    void SaveProject(wxFileName path);
    void OpenProject(wxFileName path);

protected:
    Workspace* m_workspace;

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
};

#endif // FILEHANDING_H
