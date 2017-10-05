/*
 *  Copyright (C) 2017  Thales Lima Oliveira <thales@ufu.br>
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

#include "ElementDataObject.h"

ElementDataObject::ElementDataObject() : wxDataObjectSimple(wxDataFormat("PSPCopy"))
{
    m_elementsLists = new ElementsLists();
}

ElementDataObject::ElementDataObject(std::vector<Element*> elementList) : wxDataObjectSimple(wxDataFormat("PSPCopy"))
{
    m_elementsLists = new ElementsLists();
    if(elementList.size() > 0) {
        // Separate buses (parents) from the rest of the elements (childs).
        for(auto it = elementList.begin(), itEnd = elementList.end(); it != itEnd; ++it) {
            Element* copy = (*it)->GetCopy();
            if(copy) {
                if(Bus* bus = dynamic_cast<Bus*>(copy))
                    m_elementsLists->parentList.push_back(bus);
                else
                    m_elementsLists->elementList.push_back(copy);
            }
        }
    }
}

ElementDataObject::~ElementDataObject() {}
size_t ElementDataObject::GetDataSize() const { return sizeof(void*); }
bool ElementDataObject::GetDataHere(void* buf) const
{
    *(ElementsLists**)buf = m_elementsLists;
    return true;
}

bool ElementDataObject::SetData(size_t len, const void* buf)
{
    m_elementsLists = *(ElementsLists**)buf;
    return true;
}
