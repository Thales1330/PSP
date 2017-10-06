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

#ifndef ELEMENTDATAOBJECT_H
#define ELEMENTDATAOBJECT_H

#include "Workspace.h"
#include <wx/dataobj.h>

struct ElementsLists {
    std::vector<Element*> elementList;
    std::vector<Bus*> parentList;
};

/**
 * @class ElementDataObject
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 06/10/2017
 * @brief Class to store the elements in the clipboard.
 * @file ElementDataObject.h
 */
class ElementDataObject : public wxDataObjectSimple
{
   public:
    ElementDataObject();
    ElementDataObject(std::vector<Element*> elementList);
    ~ElementDataObject();

    size_t GetDataSize() const override;
    bool GetDataHere(void* buf) const override;
    bool SetData(size_t len, const void* buf) override;

    ElementsLists* GetElementsLists() { return m_elementsLists; }
   protected:
    ElementsLists* m_elementsLists = NULL;
};

#endif  // ELEMENTDATAOBJECT_H
