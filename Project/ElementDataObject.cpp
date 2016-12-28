#include "ElectricCalculation.h"
#include "ElementDataObject.h"

ElementDataObject::ElementDataObject(Workspace* workspace)
    : wxDataObjectSimple(wxDataFormat("PSPCopy"))
{
    if(workspace) {
        m_elementsLists = new ElementsLists();

        ElectricCalculation m_allElements;
        std::vector<Element*> elementsList = workspace->GetElementList();
        std::vector<Text*> textList = workspace->GetTextList();

        m_allElements.GetElementsFromList(elementsList);

        std::vector<Bus*> busList = m_allElements.GetBusList();
        for(int i = 0; i < (int)busList.size(); i++) {
            Bus* origBus = busList[i];

            // Set bus number.
            auto data = origBus->GetEletricalData();
            data.number = i;
            origBus->SetElectricalData(data);

            if(origBus->IsSelected()) {
                Bus* copyBus = new Bus();
                *copyBus = *origBus;
                m_elementsLists->busList.push_back(copyBus);
            }
        }

        std::vector<Line*> lineList = m_allElements.GetLineList();
        for(int i = 0; i < (int)lineList.size(); i++) {
            Line* origLine = lineList[i];
            if(origLine->IsSelected()) {
                Line* copyLine = new Line();
                *copyLine = *origLine;
                m_elementsLists->lineList.push_back(copyLine);
            }
        }
        
        std::vector<Transformer*> transformerList = m_allElements.GetTransformerList();
        for(int i = 0; i < (int)transformerList.size(); i++) {
            Transformer* origTransformer = transformerList[i];
            if(origTransformer->IsSelected()) {
                Transformer* copyTransformer = new Transformer();
                *copyTransformer = *origTransformer;
                m_elementsLists->transformerList.push_back(copyTransformer);
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
