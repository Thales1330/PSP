#include "FileHanding.h"

FileHanding::~FileHanding() {}

FileHanding::FileHanding(Workspace* workspace)
{
    m_workspace = workspace;
}

FileHanding::FileHanding() {}

void FileHanding::SaveProject(wxFileName path)
{
    //Erase the file (if exists or not) and write the initial data
    std::ofstream writeProjectsFile(path.GetFullPath());
    writeProjectsFile.close();

    rapidxml::xml_document<> doc;
    rapidxml::file<> xmlFile(path.GetFullPath());
    doc.parse<0>(xmlFile.data());

    rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_declaration);
    rapidxml::xml_attribute<>* ver = doc.allocate_attribute("version", "1.0");
    rapidxml::xml_attribute<>* encoding = doc.allocate_attribute("encoding", "utf-8");
    decl->append_attribute(ver);
    decl->append_attribute(encoding);
    doc.append_node(decl);

    rapidxml::xml_node<>* rootNode = doc.allocate_node(rapidxml::node_element, "Project");
    doc.append_node(rootNode);
    
    rapidxml::xml_node<>* projectNameNode = AppendNode(doc, rootNode, "Name");
    SetNodeValue(doc, projectNameNode, path.GetName());
    
    //Save all the data
    rapidxml::xml_node<>* testValues = AppendNode(doc, rootNode, "TestValues");
    rapidxml::xml_node<>* testDouble = AppendNode(doc, testValues, "TestDouble");
    SetNodeValue(doc, testDouble, 1.23456789123456789123456789);
    rapidxml::xml_node<>* testInt = AppendNode(doc, testValues, "TestInt");
    SetNodeValue(doc, testInt, 123456789);
    rapidxml::xml_node<>* testString = AppendNode(doc, testValues, "TestString");
    SetNodeValue(doc, testString, (wxString)"TESTE");

    std::ofstream writeXML(path.GetFullPath());
    writeXML << doc;
    writeXML.close();
}

void FileHanding::OpenProject(wxFileName path) {}

rapidxml::xml_node<>* FileHanding::AppendNode(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* parentNode, const char* name, rapidxml::node_type nodeType)
{
    rapidxml::xml_node<>* node = doc.allocate_node(nodeType, name);
    parentNode->append_node(node);
    return node;
}

void FileHanding::SetNodeValue(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, wxString value)
{
    node->value(doc.allocate_string(value.mb_str()));
}

void FileHanding::SetNodeValue(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, int value)
{
    node->value(doc.allocate_string(wxString::Format("%d", value)));
}

void FileHanding::SetNodeValue(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, double value)
{
    node->value(doc.allocate_string(wxString::FromCDouble(value, 13).mb_str()));
}
