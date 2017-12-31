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

#include "MathExpression.h"
#include "MathExpressionForm.h"

#define ERROR_INDICATOR 10

MathExpressionForm::MathExpressionForm(wxWindow* parent, MathExpression* mathExpr) : MathExpressionFormBase(parent)
{
    m_parent = parent;
    m_mathExpr = mathExpr;

    // Variables string.
    wxString variables = "";
    for(unsigned int i = 0; i < m_mathExpr->GetVariables().size(); ++i) {
        variables += m_mathExpr->GetVariables()[i] + " ";
    }
    variables.RemoveLast();

    // Variables text ctrl.
    wxFont variablesFont = m_textCtrlVariables->GetFont();
    variablesFont.SetWeight(wxFONTWEIGHT_BOLD);
    m_textCtrlVariables->SetFont(variablesFont);
    m_textCtrlVariables->SetForegroundColour(wxColour(160, 0, 0));
    m_textCtrlVariables->SetValue(variables);

    m_stcMathExpr->SetText(m_mathExpr->GetMathExpression());

    m_staticTextCheckStatus->SetLabel(_("No checks performed."));
    SetSintaxHighlights();
    m_stcMathExpr->SetTabWidth(3);

    // Error indicator
    m_stcMathExpr->IndicatorSetUnder(ERROR_INDICATOR, true);
    m_stcMathExpr->IndicatorSetStyle(ERROR_INDICATOR, wxSTC_INDIC_ROUNDBOX);
    m_stcMathExpr->IndicatorSetAlpha(ERROR_INDICATOR, 200);
    m_stcMathExpr->IndicatorSetUnder(ERROR_INDICATOR, true);
    m_stcMathExpr->IndicatorSetForeground(ERROR_INDICATOR, wxColor(255, 85, 0));
    m_stcMathExpr->SetIndicatorCurrent(ERROR_INDICATOR);

    // Parser error messages to translation.
    m_translatedErrorMsg.clear();
    m_translatedErrorMsg.push_back(_("Syntax error"));
    m_translatedErrorMsg.push_back(_("Mismatched parenthesis"));
    m_translatedErrorMsg.push_back(_("Missing ')'"));
    m_translatedErrorMsg.push_back(_("Empty parentheses"));
    m_translatedErrorMsg.push_back(_("Syntax error: Operator expected"));
    m_translatedErrorMsg.push_back(_("Not enough memory"));
    m_translatedErrorMsg.push_back(_("An unexpected error occurred"));
    m_translatedErrorMsg.push_back(_("Syntax error in input variables"));
    m_translatedErrorMsg.push_back(_("Illegal number of parameters to function"));
    m_translatedErrorMsg.push_back(_("Syntax error: Premature end of string"));
    m_translatedErrorMsg.push_back(_("Syntax error: Expecting ( after function"));
    m_translatedErrorMsg.push_back(_("Syntax error: Unknown identifier"));
    m_translatedErrorMsg.push_back(_("No function has been parsed yet"));
}

MathExpressionForm::~MathExpressionForm() {}

void MathExpressionForm::OnCheckButtonClick(wxCommandEvent& event) { CheckMathExpression(); }

void MathExpressionForm::OnOKButtonClick(wxCommandEvent& event)
{
    if(ValidateData()) EndModal(wxID_OK);
}

bool MathExpressionForm::ValidateData()
{
    if(!CheckMathExpression()) {
        wxMessageDialog msg(this, _("There is an error on math expression."), _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
        msg.ShowModal();
        return false;
    }
    wxString rawVariables = m_textCtrlVariables->GetValue();
    wxString var = "";
    std::vector<wxString> variables;
    for(unsigned int i = 0; i < rawVariables.length(); ++i) {
        if(rawVariables[i] == ' ') {
            variables.push_back(var);
            var = "";
        } else if((i + 1) == static_cast<unsigned int>(rawVariables.length())) {
            var += rawVariables[i];
            variables.push_back(var);
        } else {
            var += rawVariables[i];
        }
    }

    int diff = static_cast<int>(variables.size()) - static_cast<int>(m_mathExpr->GetVariables().size());

    if(diff < 0) {
        diff = std::abs(diff);
        for(int i = 0; i < diff; ++i) { m_mathExpr->RemoveInNode(); }
    } else if(diff > 0) {
        for(int i = 0; i < diff; ++i) { m_mathExpr->AddInNode(); }
    }
    m_mathExpr->SetVariables(variables);
    m_mathExpr->SetMathExpression(m_stcMathExpr->GetValue());
    m_mathExpr->UpdatePoints();
    return true;
}

void MathExpressionForm::SetSintaxHighlights()
{
    wxString variables = m_textCtrlVariables->GetValue();

    m_stcMathExpr->StyleClearAll();
    m_stcMathExpr->SetLexer(wxSTC_LEX_DMAP);
    m_stcMathExpr->StyleSetForeground(wxSTC_DMAP_OPERATOR, wxColour(180, 0, 255));
    m_stcMathExpr->StyleSetForeground(wxSTC_DMAP_WORD, wxColour(10, 0, 255));
    m_stcMathExpr->StyleSetForeground(wxSTC_DMAP_WORD2, wxColour(160, 0, 0));
    // m_stcMathExpr->StyleSetForeground(wxSTC_DMAP_WORD3, wxColour(0, 150, 0));
    m_stcMathExpr->StyleSetForeground(wxSTC_DMAP_NUMBER, wxColour(0, 150, 0));

    m_stcMathExpr->StyleSetBold(wxSTC_DMAP_WORD, true);
    m_stcMathExpr->StyleSetBold(wxSTC_DMAP_WORD2, true);
    m_stcMathExpr->StyleSetBold(wxSTC_DMAP_WORD3, true);

    m_stcMathExpr->SetKeyWords(
        0, wxT("abs acos acosh arg asin asinh atan atan2 atanh cbrt conj ceil cos cosh cot csc eval exp exp2 floor "
               "hypot if imag int log log2 log10 max min polar pow real sec sin sinh sqrt tan tanh trunc"));
    m_stcMathExpr->SetKeyWords(1, wxT("time step switch ") + variables);
    m_stcMathExpr->SetKeyWords(2, wxT("true false open closed pi e"));
}

void MathExpressionForm::OnTextUpdate(wxCommandEvent& event) { SetSintaxHighlights(); }

std::string MathExpressionForm::GetVariablesToParse()
{
    wxString variables = wxT("time,step,switch,") + m_textCtrlVariables->GetValue();
    for(unsigned int i = 0; i < variables.length(); ++i) {
        if(variables[i] == ' ') variables[i] = ',';
    }
    return static_cast<std::string>(variables);
}

void MathExpressionForm::OnLeftClickDown(wxMouseEvent& event)
{
    m_stcMathExpr->IndicatorClearRange(0, m_stcMathExpr->GetValue().length());
    event.Skip();
}

bool MathExpressionForm::CheckMathExpression()
{
    bool success = true;
    m_stcMathExpr->IndicatorClearRange(0, m_stcMathExpr->GetValue().length());
    int currentLang = wxLocale::GetSystemLanguage();
    wxLocale newLocale(wxLANGUAGE_ENGLISH_US);
    MathExprParser parser = m_mathExpr->GetParser();
    int parserRes = parser.Parse(static_cast<std::string>(m_stcMathExpr->GetValue()), GetVariablesToParse());
    if(parserRes != -1) {
        m_staticTextCheckStatus->SetLabel(m_translatedErrorMsg[parser.GetParseErrorType()]);
        m_staticTextCheckStatus->SetForegroundColour(wxColor(255, 0, 0));
        m_stcMathExpr->IndicatorFillRange(parserRes, 1);
        success = false;
    } else {
        m_staticTextCheckStatus->SetLabel(_("The math expression is correct."));
        m_staticTextCheckStatus->SetForegroundColour(wxColor(0, 128, 0));
    }
    wxLocale oldLocale(currentLang);
    Layout();
    return success;
}
