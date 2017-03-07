#include "SumForm.h"
#include "Sum.h"

SumForm::SumForm(wxWindow* parent, Sum* sum)
    : SumFormBase(parent)
{
    m_parent = parent;
    m_sum = sum;
}

SumForm::~SumForm()
{
}

