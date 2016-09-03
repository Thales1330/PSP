#include "IndMotor.h"

IndMotor::IndMotor() : Machines()
{
}

IndMotor::~IndMotor()
{
}

void IndMotor::DrawSymbol() const
{
	std::vector<wxPoint2DDouble> mPts;
	mPts.push_back(wxPoint2DDouble(-10, 13) + m_position);
	mPts.push_back(wxPoint2DDouble(-10, -13) + m_position);
	mPts.push_back(wxPoint2DDouble(0, 2) + m_position);
	mPts.push_back(wxPoint2DDouble(10, -13) + m_position);
	mPts.push_back(wxPoint2DDouble(10, 13) + m_position);
	DrawLine(mPts);
}

bool IndMotor::GetContextMenu(wxMenu& menu)
{
	menu.Append(ID_EDIT_INDMOTOR, _("Edit induction motor"));
    menu.Append(ID_ROTATE, _("Rotate"));
    menu.Append(ID_DELETE, _("Delete"));
    return true;
}
