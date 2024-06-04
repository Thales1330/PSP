/*
 *  Copyright (C) 2017  Thales Lima Oliveira <thales@ufu.br>
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

#include "SwitchingForm.h"
#include "../elements/powerElement/PowerElement.h"

SwitchingForm::SwitchingForm(wxWindow* parent) : SwitchingFormBase(parent)
{
    m_listCtrlSwitchings->AppendColumn(_("Type"));
    m_listCtrlSwitchings->AppendColumn(_("Time (s)"));

    SetSize(GetBestSize());
    Layout();
}

SwitchingForm::SwitchingForm(wxWindow* parent, PowerElement* element) : SwitchingFormBase(parent)
{
    m_listCtrlSwitchings->AppendColumn(_("Type"));
    m_listCtrlSwitchings->AppendColumn(_("Time (s)"));

    SetSize(GetBestSize());
    Layout();

    SwitchingData data = element->GetSwitchingData();
    for(int i = 0; i < (int)data.swType.size(); i++) {
        long index = m_listCtrlSwitchings->InsertItem(m_maxID, data.swType[i] == SwitchingType::SW_INSERT ? _("Insert") : _("Remove"));
        m_listCtrlSwitchings->SetItem(index, 1, wxString::FromDouble(data.swTime[i]));
        m_maxID++;
    }

    m_element = element;
}

SwitchingForm::~SwitchingForm() {}
void SwitchingForm::OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
void SwitchingForm::OnInsertButtonClick(wxCommandEvent& event)
{
    long index = m_listCtrlSwitchings->InsertItem(
        m_maxID, m_pgPropType->GetValue().GetInteger() == 0 ? _("Insert") : _("Remove"));
    m_listCtrlSwitchings->SetItem(index, 1, m_pgPropTime->GetValue().GetString());
    m_maxID++;
}
void SwitchingForm::OnOKButtonClick(wxCommandEvent& event)
{
    std::vector<long> itemList;
    long item = -1;
    while(true) {
        item = m_listCtrlSwitchings->GetNextItem(item);
        if(item == -1) break;
        itemList.push_back(item);
    }

    SwitchingData data;
    for(int i = 0; i < (int)itemList.size(); i++) {
        if(m_listCtrlSwitchings->GetItemText(itemList[i], 0) == _("Insert"))
            data.swType.push_back(SwitchingType::SW_INSERT);
        else
            data.swType.push_back(SwitchingType::SW_REMOVE);

        double swTime;
        m_listCtrlSwitchings->GetItemText(itemList[i], 1).ToDouble(&swTime);
        data.swTime.push_back(swTime);
    }
    m_element->SetSwitchingData(data);

    if(data.swTime.size() != 0)
        m_element->SetDynamicEvent(true);
    else
        m_element->SetDynamicEvent(false);

    EndModal(wxID_OK);
}

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
void SwitchingForm::OnDownButtonClick(wxCommandEvent& event)
{
    std::vector<long> selectedList;
    std::vector<long> itemList;
    long item = -1;
    while(true) {
        item = m_listCtrlSwitchings->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if(item == -1) break;
        selectedList.push_back(item);
    }
    while(true) {
        item = m_listCtrlSwitchings->GetNextItem(item);
        if(item == -1) break;
        itemList.push_back(item);
    }

    for(int i = 1; i < (int)itemList.size(); i++) {
        for(int j = 0; j < (int)selectedList.size(); j++) {
            if(itemList[i - 1] == selectedList[j]) {
                wxString col1Str[2];
                wxString col2Str[2];

                col1Str[0] = m_listCtrlSwitchings->GetItemText(itemList[i], 0);
                col1Str[1] = m_listCtrlSwitchings->GetItemText(selectedList[j], 0);
                col2Str[0] = m_listCtrlSwitchings->GetItemText(itemList[i], 1);
                col2Str[1] = m_listCtrlSwitchings->GetItemText(selectedList[j], 1);

                m_listCtrlSwitchings->SetItem(itemList[i], 0, col1Str[1]);
                m_listCtrlSwitchings->SetItem(selectedList[j], 0, col1Str[0]);
                m_listCtrlSwitchings->SetItem(itemList[i], 1, col2Str[1]);
                m_listCtrlSwitchings->SetItem(selectedList[j], 1, col2Str[0]);

                m_listCtrlSwitchings->SetItemState(itemList[i], wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
                m_listCtrlSwitchings->SetItemState(selectedList[j], 0, wxLIST_STATE_SELECTED);

                i++;
                break;
            }
        }
    }
}
void SwitchingForm::OnUpButtonClick(wxCommandEvent& event)
{
    std::vector<long> selectedList;
    std::vector<long> itemList;
    long item = -1;
    while(true) {
        item = m_listCtrlSwitchings->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if(item == -1) break;
        selectedList.push_back(item);
    }
    while(true) {
        item = m_listCtrlSwitchings->GetNextItem(item);
        if(item == -1) break;
        itemList.push_back(item);
    }

    for(int i = 0; i < (int)itemList.size(); i++) {
        for(int j = 0; j < (int)selectedList.size(); j++) {
            if(i + 1 < (int)itemList.size()) {
                if(itemList[i + 1] == selectedList[j]) {
                    wxString col1Str[2];
                    wxString col2Str[2];

                    col1Str[0] = m_listCtrlSwitchings->GetItemText(itemList[i], 0);
                    col1Str[1] = m_listCtrlSwitchings->GetItemText(selectedList[j], 0);
                    col2Str[0] = m_listCtrlSwitchings->GetItemText(itemList[i], 1);
                    col2Str[1] = m_listCtrlSwitchings->GetItemText(selectedList[j], 1);

                    m_listCtrlSwitchings->SetItem(itemList[i], 0, col1Str[1]);
                    m_listCtrlSwitchings->SetItem(selectedList[j], 0, col1Str[0]);
                    m_listCtrlSwitchings->SetItem(itemList[i], 1, col2Str[1]);
                    m_listCtrlSwitchings->SetItem(selectedList[j], 1, col2Str[0]);

                    m_listCtrlSwitchings->SetItemState(itemList[i], wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
                    m_listCtrlSwitchings->SetItemState(selectedList[j], 0, wxLIST_STATE_SELECTED);

                    i++;
                    break;
                }
            }
        }
    }
}
