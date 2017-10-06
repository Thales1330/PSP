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

#include "SumForm.h"
#include "Sum.h"

SumForm::SumForm(wxWindow* parent, Sum* sum) : SumFormBase(parent)
{
    SetSize(GetBestSize());

    m_parent = parent;
    m_sum = sum;

    wxString signalStr = "";
    auto signalList = m_sum->GetSignalList();
    for(auto it = signalList.begin(), itEnd = signalList.end(); it != itEnd; ++it) {
        Sum::Signal signal = *it;
        switch(signal) {
            case Sum::SIGNAL_POSITIVE: {
                signalStr += "+";
            } break;
            case Sum::SIGNAL_NEGATIVE: {
                signalStr += "-";
            } break;
        }
        if(it != itEnd - 1) signalStr += " ";
    }
    m_textCtrlSigns->SetValue(signalStr);
}

SumForm::~SumForm() {}
void SumForm::OnOKClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

bool SumForm::ValidateData()
{
    wxString signalStr = "";
    for(int i = 0; i < (int)m_textCtrlSigns->GetValue().length(); ++i) {
        if(m_textCtrlSigns->GetValue()[i] != ' ') signalStr += m_textCtrlSigns->GetValue()[i];
    }
    if(signalStr.size() < 2) {
        wxMessageDialog msg(this, _("You must assign at least two signals."), _("Error"),
                            wxOK | wxCENTRE | wxICON_ERROR);
        msg.ShowModal();
        return false;
    }

    std::vector<Sum::Signal> signalList;
    for(int i = 0; i < (int)signalStr.length(); ++i) {
        switch(signalStr[i].GetValue()) {
            case '+': {
                signalList.push_back(Sum::SIGNAL_POSITIVE);
            } break;
            case '-': {
                signalList.push_back(Sum::SIGNAL_NEGATIVE);
            } break;
            default: {
                wxMessageDialog msg(this, _("Value entered incorrectly in the field \"Signs\"."), _("Error"),
                                    wxOK | wxCENTRE | wxICON_ERROR);
                msg.ShowModal();
                return false;
            }
        }
    }

    int diff = (int)signalList.size() - (int)m_sum->GetSignalList().size();

    if(diff < 0) {
        diff = std::abs(diff);
        for(int i = 0; i < diff; ++i) {
            m_sum->RemoveInNode();
        }
    } else if(diff > 0) {
        for(int i = 0; i < diff; ++i) {
            m_sum->AddInNode();
        }
    }
    m_sum->SetSignalList(signalList);
    m_sum->UpdatePoints();
    return true;
}
