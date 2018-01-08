/*
 *  Copyright (C) 2018  Thales Lima Oliveira <thales@ufu.br>
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

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "rapidXML/rapidxml.hpp"
// Modified: http://stackoverflow.com/questions/14113923/rapidxml-print-header-has-undefined-methods
#include "rapidXML/rapidxml_print.hpp"
#include "rapidXML/rapidxml_utils.hpp"

#include <wx/string.h>

/**
 * @class XMLParser
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 08/01/2018
 * @brief XML parser to save and open project and control files.
 * @file XMLParser.h
 */
class XMLParser
{
   public:
    XMLParser();
    ~XMLParser();

    static rapidxml::xml_node<>* AppendNode(rapidxml::xml_document<>& doc,
                                            rapidxml::xml_node<>* parentNode,
                                            const char* name,
                                            rapidxml::node_type nodeType = rapidxml::node_element);
    static void SetNodeValue(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, wxString value);
    static void SetNodeValue(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, int value);
    static void SetNodeValue(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* node, double value);
    static void SetNodeAttribute(rapidxml::xml_document<>& doc,
                                 rapidxml::xml_node<>* node,
                                 const char* atrName,
                                 wxString value);
    static void SetNodeAttribute(rapidxml::xml_document<>& doc,
                                 rapidxml::xml_node<>* node,
                                 const char* atrName,
                                 int value);
    static void SetNodeAttribute(rapidxml::xml_document<>& doc,
                                 rapidxml::xml_node<>* node,
                                 const char* atrName,
                                 double value);
    static double GetNodeValueDouble(rapidxml::xml_node<>* parent, const char* nodeName);
    static int GetNodeValueInt(rapidxml::xml_node<>* parent, const char* nodeName);
    static int GetAttributeValueInt(rapidxml::xml_node<>* parent, const char* nodeName, const char* atrName);
    static int GetAttributeValueInt(rapidxml::xml_node<>* node, const char* atrName);
};

#endif  // XMLPARSER_H
