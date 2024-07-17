#include "ATPFileEditorForm.h"
#include <wx/textfile.h>

ATPFileEditorForm::ATPFileEditorForm(wxWindow* parent, wxString path)
    : ATPFileEditorFormBase(parent), m_path(path)
{
    wxTextFile file;
    file.Open(m_path);
    m_stcATPFile->AppendText(file.GetFirstLine() + "\n");
    while (!file.Eof())
    {
        m_stcATPFile->AppendText(file.GetNextLine() + "\n");
    }
    file.Close();
    m_stcATPFile->StyleClearAll();
    m_stcATPFile->SetLexer(wxSTC_LEX_F77);
    //m_stcATPFile->StyleSetForeground(wxSTC_F_STRING1, wxColour(150, 0, 0));
    //m_stcATPFile->StyleSetForeground(wxSTC_F_IDENTIFIER, wxColour(40, 0, 60));
    //m_stcATPFile->StyleSetForeground(wxSTC_F_NUMBER, wxColour(255, 100, 0));
    //m_stcATPFile->StyleSetForeground(wxSTC_F_LABEL, wxColour(255, 0, 0));
    //m_stcATPFile->StyleSetForeground(wxSTC_F_OPERATOR, wxColour(150, 0, 150));
    //m_stcATPFile->StyleSetForeground(wxSTC_F_WORD, wxColour(0, 0, 150));
    //m_stcATPFile->StyleSetForeground(wxSTC_F_WORD2, wxColour(255, 165, 0));
    m_stcATPFile->StyleSetForeground(wxSTC_F_COMMENT, wxColour(0, 100, 25));
    //m_stcATPFile->StyleSetBold(wxSTC_P_WORD, true);
    //m_stcATPFile->StyleSetBold(wxSTC_P_WORD2, true);
    // Set max length to 80 characters
    m_stcATPFile->SetEdgeColumn(80);
    m_stcATPFile->SetEdgeMode(wxSTC_EDGE_LINE);

    //m_stcATPFile->SetKeyWords(0, wxT("BEGIN /SOURCE"));
}

ATPFileEditorForm::~ATPFileEditorForm()
{
}

void ATPFileEditorForm::OnCancelClick(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}
void ATPFileEditorForm::OnOKClick(wxCommandEvent& event)
{
    wxTextFile file;
    file.Open(m_path);
	file.Clear();
	file.AddLine(m_stcATPFile->GetText());
	file.Write();
	file.Close();
	EndModal(wxID_OK);
}
