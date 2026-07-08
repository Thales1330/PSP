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

#include "ControlElementDataObject.h"

ControlElementDataObject::ControlElementDataObject()
	: wxDataObjectSimple(wxDataFormat(wxT("PSPControlCopy")))
{
	// When used for reading clipboard data, we do NOT own the lists by default.
	m_elementsLists = nullptr;
	m_ownsLists = false;
}

ControlElementDataObject::ControlElementDataObject(
	const std::vector<std::shared_ptr<ControlElement>>& elementList,
	const std::vector<std::shared_ptr<ConnectionLine>>& connectionList)
	: wxDataObjectSimple(wxDataFormat(wxT("PSPControlCopy")))
{
	m_elementsLists = new ControlElementsLists();
	m_elementsLists->elementList = elementList;
	m_elementsLists->connectionList = connectionList;
	m_ownsLists = true;
}

ControlElementDataObject::~ControlElementDataObject()
{
	if (m_ownsLists && m_elementsLists) {
		delete m_elementsLists;
		m_elementsLists = nullptr;
	}
}

size_t ControlElementDataObject::GetDataSize() const
{
	return sizeof(void*);
}

bool ControlElementDataObject::GetDataHere(void* buf) const
{
	*(ControlElementsLists**)buf = m_elementsLists;
	return true;
}

bool ControlElementDataObject::SetData(size_t len, const void* buf)
{
	(void)len;
	m_elementsLists = *(ControlElementsLists**)buf;
	// The clipboard-owned data object owns the lists, not this stack reader.
	m_ownsLists = false;
	return true;
}

