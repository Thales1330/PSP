#include "SumForm.h"
#include "Sum.h"

SumForm::SumForm(wxWindow* parent, Sum* sum)
    : SumFormBase(parent)
{
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
    if(signalStr.size() < 2) return false;

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
                return false;
            }
        }
    }

    int diff = (int)signalList.size() - (int)m_sum->GetSignalList().size();

    if(diff < 0) {

    } else if(diff > 0) {
        auto nodeList = m_sum->GetNodeList();
        for(int i = 0; i < diff; ++i) {
            Node* newNode = new Node();
            nodeList.insert(nodeList.end() - 1, newNode);
        }
        m_sum->SetNodeList(nodeList);
    }
    m_sum->SetSignalList(signalList);
    m_sum->UpdatePoints();
    return true;
}
