#ifndef ELEMENTDATAOBJECT_H
#define ELEMENTDATAOBJECT_H

#include "Workspace.h"
#include <wx/dataobj.h>

struct ElementsLists {
	std::vector<Element*> elementList;
    std::vector<Bus*> parentList;
};

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
    ElementsLists* m_elementsLists;
};

#endif // ELEMENTDATAOBJECT_H
