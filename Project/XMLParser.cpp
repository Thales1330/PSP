#include "XMLParser.h"

XMLParser::XMLParser() {}

XMLParser::~XMLParser() {}

rapidxml::xml_node<>* XMLParser::AppendNode(rapidxml::xml_document<>& doc,
                                            rapidxml::xml_node<>* parentNode,
                                            const char* name,
                                            rapidxml::node_type nodeType)
{
    rapidxml::xml_node<>* node = doc.allocate_node(nodeType, name);
    parentNode->append_node(node);
    return node;
}

void XMLParser::SetNodeValue(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, wxString value)
{
    node->value(doc.allocate_string(value.mb_str()));
}

void XMLParser::SetNodeValue(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, int value)
{
    node->value(doc.allocate_string(wxString::Format("%d", value).mb_str()));
}

void XMLParser::SetNodeValue(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, double value)
{
    node->value(doc.allocate_string(wxString::FromCDouble(value, 13).mb_str()));
}

void XMLParser::SetNodeAttribute(rapidxml::xml_document<>& doc,
                                 rapidxml::xml_node<>* node,
                                 const char* atrName,
                                 wxString value)
{
    node->append_attribute(doc.allocate_attribute(atrName, doc.allocate_string(value.mb_str())));
}

void XMLParser::SetNodeAttribute(rapidxml::xml_document<>& doc,
                                 rapidxml::xml_node<>* node,
                                 const char* atrName,
                                 int value)
{
    node->append_attribute(
        doc.allocate_attribute(atrName, doc.allocate_string(wxString::Format("%d", value).mb_str())));
}

void XMLParser::SetNodeAttribute(rapidxml::xml_document<>& doc,
                                 rapidxml::xml_node<>* node,
                                 const char* atrName,
                                 double value)
{
    node->append_attribute(
        doc.allocate_attribute(atrName, doc.allocate_string(wxString::FromCDouble(value, 13).mb_str())));
}

double XMLParser::GetNodeValueDouble(rapidxml::xml_node<>* parent, const char* nodeName)
{
    double dValue = 0.0;
    if(parent) {
        auto node = parent->first_node(nodeName);
        if(node) wxString(node->value()).ToCDouble(&dValue);
    }
    return dValue;
}

int XMLParser::GetNodeValueInt(rapidxml::xml_node<>* parent, const char* nodeName)
{
    long iValue = -1;
    if(parent) {
        auto node = parent->first_node(nodeName);
        if(node) wxString(node->value()).ToCLong(&iValue);
    }
    return (int)iValue;
}

int XMLParser::GetAttributeValueInt(rapidxml::xml_node<>* parent, const char* nodeName, const char* atrName)
{
    long iValue = -1;
    if(parent) {
        auto node = parent->first_node(nodeName);
        if(node) {
            auto atr = node->first_attribute(atrName);
            if(atr) wxString(atr->value()).ToCLong(&iValue);
        }
    }
    return (int)iValue;
}

int XMLParser::GetAttributeValueInt(rapidxml::xml_node<>* node, const char* atrName)
{
    long intValue;
    auto atr = node->first_attribute(atrName);
    if(!atr) return false;
    wxString(atr->value()).ToCLong(&intValue);
    return (int)intValue;
}
