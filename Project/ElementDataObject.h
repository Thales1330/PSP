#ifndef ELEMENTDATAOBJECT_H
#define ELEMENTDATAOBJECT_H

#include <wx/dataobj.h>
#include "Workspace.h"

class ElectricCalculation;

struct ElementsLists {
    std::vector<Bus*> busList;
    std::vector<Capacitor*> capacitorList;
    std::vector<IndMotor*> indMotorList;
    std::vector<Inductor*> inductorList;
    std::vector<Line*> lineList;
    std::vector<Load*> loadList;
    std::vector<Machines*> machinesList;
    std::vector<SyncGenerator*> syncGeneratorList;
    std::vector<SyncMotor*> syncMotorList;
    std::vector<Transformer*> transformerList;
    std::vector<Text*> textList;
};

class ElementDataObject : public wxDataObjectSimple
{
public:
    ElementDataObject(Workspace* workspace);
    ~ElementDataObject();

    size_t GetDataSize() const override;
    bool GetDataHere(void* buf) const override;
    bool SetData(size_t len, const void* buf) override;
    
    ElementsLists* GetElementsLists() { return m_elementsLists; }

protected:
    ElementsLists* m_elementsLists;
};

#endif // ELEMENTDATAOBJECT_H
