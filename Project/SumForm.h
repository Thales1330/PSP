#ifndef SUMFORM_H
#define SUMFORM_H

#include "ElementForm.h"

class Sum;

class SumForm : public SumFormBase
{
public:
    SumForm(wxWindow* parent, Sum* sum);
    virtual ~SumForm();
    
protected:
    wxWindow* m_parent = NULL;
    Sum* m_sum = NULL;
};
#endif // SUMFORM_H
