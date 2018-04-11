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

#ifndef MATHEXPRESSIONFORM_H
#define MATHEXPRESSIONFORM_H

#include "ElementFormBase.h"
#include <wx/msgdlg.h>

class MathExpression;

class MathExpressionForm : public MathExpressionFormBase
{
public:
    MathExpressionForm(wxWindow* parent, MathExpression* mathExpr);
    virtual ~MathExpressionForm();
protected:
    bool ValidateData();
    void SetSintaxHighlights();
    bool CheckMathExpression();
    std::string GetVariablesToParse();
    virtual void OnCancelButtonClick(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
    virtual void OnCheckButtonClick(wxCommandEvent& event);
    virtual void OnOKButtonClick(wxCommandEvent& event);
    virtual void OnTextEnter(wxCommandEvent& event) {}
    virtual void OnTextUpdate(wxCommandEvent& event);
    virtual void OnLeftClickDown(wxMouseEvent& event);
    
    std::vector<wxString> m_translatedErrorMsg;
    
    wxWindow* m_parent;
    MathExpression* m_mathExpr;
};
#endif // MATHEXPRESSIONFORM_H
