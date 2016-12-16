#include "ElectricCalculation.h"
#include "ElementDataObject.h"

ElementDataObject::ElementDataObject(Workspace* workspace) : wxDataObjectSimple(wxDataFormat("PSPCopy"))
{
    m_allElements.GetElementsFromList(workspace->GetElementList());
    std::vector<Text*> textList = workspace->GetTextList();
    
    std::vector<Bus*> busList = m_allElements.GetBusList();
    for(int i = 0; i<(int)busList.size(); i++) {
        Bus* origBus = busList[i];
        if(origBus->IsSelected()) {
            Bus* copyBus = new Bus();
            *copyBus = *origBus;
            m_elementsLists->busList.push_back(copyBus);
        }
    }
}

ElementDataObject::~ElementDataObject()
{
}

size_t ElementDataObject::GetDataSize() const
{
    return sizeof(void*);
}

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
