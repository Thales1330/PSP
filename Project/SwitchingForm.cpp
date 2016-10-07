#include "SwitchingForm.h"

SwitchingForm::SwitchingForm(wxWindow* parent) : SwitchingFormBase(parent)
{
    m_listCtrlSwitchings->AppendColumn(_("Type"));
    m_listCtrlSwitchings->AppendColumn(_("Time (s)"));

    SetSize(GetBestSize());
    Layout();

    /*for(int i=0; i<10; i++) {
        m_listCtrlSwitchings->InsertItem(i, "Entrada");
        m_listCtrlSwitchings->SetItem(i, 1, wxString::Format("%d", i));
    }*/
}

SwitchingForm::~SwitchingForm() {}
void SwitchingForm::OnCancelButtonClick(wxCommandEvent& event) {}
void SwitchingForm::OnInsertButtonClick(wxCommandEvent& event)
{
    long index = m_listCtrlSwitchings->InsertItem(
        m_maxID, m_pgPropType->GetValue().GetInteger() == 0 ? _("Insert") : _("Remove"));
    m_listCtrlSwitchings->SetItem(index, 1, m_pgPropTime->GetValue().GetString());
    m_maxID++;
}
void SwitchingForm::OnOKButtonClick(wxCommandEvent& event) {}
void SwitchingForm::OnRemoveButtonClick(wxCommandEvent& event)
{
    std::vector<long> itemList;
    long item = -1;
    while(true) {
        item = m_listCtrlSwitchings->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if(item == -1) break;
        itemList.push_back(item);
    }
    for(int i = (int)itemList.size() - 1; i >= 0; i--) {
        m_listCtrlSwitchings->DeleteItem(itemList[i]);
    }
}
void SwitchingForm::OnChangeProperties(wxPropertyGridEvent& event) {}
void SwitchingForm::OnSelectItem(wxListEvent& event) {}
