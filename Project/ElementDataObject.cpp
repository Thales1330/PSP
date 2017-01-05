#include "ElementDataObject.h"

ElementDataObject::ElementDataObject()
    : wxDataObjectSimple(wxDataFormat("PSPCopy"))
{
    m_elementsLists = new ElementsLists();
}

ElementDataObject::ElementDataObject(std::vector<Element*> elementList)
    : wxDataObjectSimple(wxDataFormat("PSPCopy"))
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
