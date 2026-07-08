/*
 *  Copyright (C) 2026 Thales Lima Oliveira <thales.oliveira@uftm.edu.br>
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

#ifndef CONTROLELEMENTDATAOBJECT_H
#define CONTROLELEMENTDATAOBJECT_H

#include <memory>
#include <vector>

#include <wx/dataobj.h>

class ControlElement;
class ConnectionLine;

struct ControlElementsLists {
	std::vector<std::shared_ptr<ControlElement>> elementList;
	std::vector<std::shared_ptr<ConnectionLine>> connectionList;
};

/**
 * @class ControlElementDataObject
 * @author Thales Lima Oliveira <thales.oliveira@uftm.edu.br>
 * @date 08/07/2026
 * @brief Clipboard payload for ControlEditor copy/paste (control elements + connection lines).
 *
 * Uses the same pointer-passing pattern as ElementDataObject, but with explicit ownership
 * control to avoid double-free when reading clipboard data.
 */
class ControlElementDataObject : public wxDataObjectSimple
{
public:
	ControlElementDataObject();
	ControlElementDataObject(
		const std::vector<std::shared_ptr<ControlElement>>& elementList,
		const std::vector<std::shared_ptr<ConnectionLine>>& connectionList);
	~ControlElementDataObject() override;

	size_t GetDataSize() const override;
	bool GetDataHere(void* buf) const override;
	bool SetData(size_t len, const void* buf) override;

	ControlElementsLists* GetElementsLists() { return m_elementsLists; }

private:
	ControlElementsLists* m_elementsLists = nullptr;
	bool m_ownsLists = false;
};

#endif  // CONTROLELEMENTDATAOBJECT_H

