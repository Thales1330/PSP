#include "HarmCurrent.h"
#include "HarmCurrentForm.h"

HarmCurrentForm::HarmCurrentForm(wxWindow* parent, HarmCurrent* harmCurrent) : HarmCurrentFormBase(parent)
{
    m_parent = parent;
    m_harmCurrent = harmCurrent;

    m_listCtrlHarmCurrentList->AppendColumn(_("Order"));
    m_listCtrlHarmCurrentList->AppendColumn(_("Current"));
    m_listCtrlHarmCurrentList->AppendColumn(_("Unit"));
    m_listCtrlHarmCurrentList->AppendColumn(_("Angle"));

    auto data = m_harmCurrent->GetElectricalData();
    m_textCtrlName->SetValue(data.name);
    for(unsigned int i = 0; i < data.harmonicOrder.size(); ++i) {
        long item = m_listCtrlHarmCurrentList->InsertItem(m_maxID, wxString::Format("%d", data.harmonicOrder[i]));
        m_listCtrlHarmCurrentList->SetItem(item, 1, m_harmCurrent->StringFromDouble(data.injHarmCurrent[i], 1));
        m_listCtrlHarmCurrentList->SetItem(item, 2, data.injHarmCurrentUnit[i] == ElectricalUnit::UNIT_A ? _("A") : _("p.u."));
        m_listCtrlHarmCurrentList->SetItem(item, 3, m_harmCurrent->StringFromDouble(data.injHarmAngle[i], 1));
        m_maxID++;
    }

    SetSize(GetBestSize());
    Layout();
}

HarmCurrentForm::~HarmCurrentForm() {}

void HarmCurrentForm::OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
void HarmCurrentForm::OnOKButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

bool HarmCurrentForm::ValidateData()
{
    wxString name = m_textCtrlName->GetValue();
    std::vector<int> harmonicOrder;
    std::vector<double> injHarmCurrent;
    std::vector<ElectricalUnit> injHarmCurrentUnit;
    std::vector<double> injHarmAngle;

    long item = -1;
    while(true) {
        item = m_listCtrlHarmCurrentList->GetNextItem(item, wxLIST_NEXT_ALL);
        if(item == -1) break;
        harmonicOrder.push_back(wxAtoi(m_listCtrlHarmCurrentList->GetItemText(item, 0)));
        double injCurrent = 0.0;
        if(!m_listCtrlHarmCurrentList->GetItemText(item, 1).ToDouble(&injCurrent)) { return false; }
        injHarmCurrent.push_back(injCurrent);
        if(m_listCtrlHarmCurrentList->GetItemText(item, 2) == "A")
            injHarmCurrentUnit.push_back(ElectricalUnit::UNIT_A);
        else
            injHarmCurrentUnit.push_back(ElectricalUnit::UNIT_PU);
        double injCAngle = 0.0;
        if(!m_listCtrlHarmCurrentList->GetItemText(item, 3).ToDouble(&injCAngle)) { return false; }
        injHarmAngle.push_back(injCAngle);
    }

    auto data = m_harmCurrent->GetElectricalData();
    data.name = name;
    data.harmonicOrder = harmonicOrder;
    data.injHarmCurrent = injHarmCurrent;
    data.injHarmCurrentUnit = injHarmCurrentUnit;
    data.injHarmAngle = injHarmAngle;
    m_harmCurrent->SetElectricalData(data);

    return true;
}
void HarmCurrentForm::OnAddButtonClick(wxCommandEvent& event)
{
    wxString order = m_pgPropHarmOrder->GetValue().GetString();  // Get order in properties
    // Look if already have the order in the list
    long item = -1;
    bool findOrder = false;
    while(true) {
        item = m_listCtrlHarmCurrentList->GetNextItem(item, wxLIST_NEXT_ALL);
        if(item == -1) break;
        if(m_listCtrlHarmCurrentList->GetItemText(item, 0) == order) {
            findOrder = true;
            break;
        }
    }

    if(!findOrder) item = m_listCtrlHarmCurrentList->InsertItem(m_maxID, m_pgPropHarmOrder->GetValue().GetString());
    m_listCtrlHarmCurrentList->SetItem(item, 1, m_pgPropHarmCurrent->GetValue().GetString());
    m_listCtrlHarmCurrentList->SetItem(item, 2, m_pgPropUnit->GetValue().GetInteger() == 0 ? _("A") : _("p.u."));
    m_listCtrlHarmCurrentList->SetItem(item, 3, m_pgPropHarmAngle->GetValue().GetString());
    if(!findOrder) m_maxID++;

    SortList();
}
void HarmCurrentForm::OnRemoveButtonClick(wxCommandEvent& event)
{
    std::vector<long> itemList;
    long item = -1;
    while(true) {
        item = m_listCtrlHarmCurrentList->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if(item == -1) break;
        itemList.push_back(item);
    }
    for(int i = static_cast<int>(itemList.size()) - 1; i >= 0; --i) {
        m_listCtrlHarmCurrentList->DeleteItem(itemList[i]);
    }
}

void HarmCurrentForm::SortList()
{
    // Get data, sort and recreate list.

    std::vector<int> harmonicOrder;
    std::vector<double> injHarmCurrent;
    std::vector<ElectricalUnit> injHarmCurrentUnit;
    std::vector<double> injHarmAngle;

    // Get data
    long item = -1;
    while(true) {
        item = m_listCtrlHarmCurrentList->GetNextItem(item, wxLIST_NEXT_ALL);
        if(item == -1) break;
        harmonicOrder.push_back(wxAtoi(m_listCtrlHarmCurrentList->GetItemText(item, 0)));
        double injCurrent = 0.0;
        if(!m_listCtrlHarmCurrentList->GetItemText(item, 1).ToDouble(&injCurrent)) return;
        injHarmCurrent.push_back(injCurrent);
        if(m_listCtrlHarmCurrentList->GetItemText(item, 2) == "A")
            injHarmCurrentUnit.push_back(ElectricalUnit::UNIT_A);
        else
            injHarmCurrentUnit.push_back(ElectricalUnit::UNIT_PU);
        double injCAngle = 0.0;
        if(!m_listCtrlHarmCurrentList->GetItemText(item, 3).ToDouble(&injCAngle)) return;
        injHarmAngle.push_back(injCAngle);
    }

    // Sort
    for(unsigned int i = 0; i < harmonicOrder.size(); ++i) {
        for(unsigned int j = 0; j < harmonicOrder.size(); ++j) {
            if(harmonicOrder[i] < harmonicOrder[j]) {
                int h = harmonicOrder[j];
                harmonicOrder[j] = harmonicOrder[i];
                harmonicOrder[i] = h;

                double current = injHarmCurrent[j];
                injHarmCurrent[j] = injHarmCurrent[i];
                injHarmCurrent[i] = current;

                auto unit = injHarmCurrentUnit[j];
                injHarmCurrentUnit[j] = injHarmCurrentUnit[i];
                injHarmCurrentUnit[i] = unit;

                double angle = injHarmAngle[j];
                injHarmAngle[j] = injHarmAngle[i];
                injHarmAngle[i] = angle;
            }
        }
    }

    // Clear all and reconstruct list
    m_listCtrlHarmCurrentList->ClearAll();
    m_listCtrlHarmCurrentList->AppendColumn(_("Order"));
    m_listCtrlHarmCurrentList->AppendColumn(_("Current"));
    m_listCtrlHarmCurrentList->AppendColumn(_("Unit"));
    m_listCtrlHarmCurrentList->AppendColumn(_("Angle"));
    m_maxID = 0;
    for(unsigned int i = 0; i < harmonicOrder.size(); ++i) {
        long item = m_listCtrlHarmCurrentList->InsertItem(m_maxID, wxString::Format("%d", harmonicOrder[i]));
        m_listCtrlHarmCurrentList->SetItem(item, 1, m_harmCurrent->StringFromDouble(injHarmCurrent[i], 1));
        m_listCtrlHarmCurrentList->SetItem(item, 2, injHarmCurrentUnit[i] == ElectricalUnit::UNIT_A ? _("A") : _("p.u."));
        m_listCtrlHarmCurrentList->SetItem(item, 3, m_harmCurrent->StringFromDouble(injHarmAngle[i], 1));
        m_maxID++;
    }
}
