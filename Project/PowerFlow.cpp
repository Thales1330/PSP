#include "PowerFlow.h"

PowerFlow::PowerFlow(std::vector<Element*> elementList) : ElectricCalculation()
{
    GetElementsFromList(elementList);
}

PowerFlow::~PowerFlow()
{
}

bool PowerFlow::RunGaussSeidel()
{
    GetYBus(m_yBus, 100e6);
    
    wxString str = "";
    for(int i=0; i<(int)m_yBus.size(); i++) {
        for(int j=0; j<(int)m_yBus[i].size(); j++) {
            str += wxString::Format("%.2f + j%.2f\t", m_yBus[i][j].real(), m_yBus[i][j].imag());
        }
        str += "\n";
    }
    
    wxLogMessage(str);
    
    return true;
}
