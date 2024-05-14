#ifndef DEBUGMAINFRAME_HPP
#define DEBUGMAINFRAME_HPP
#include "DebugFrameBase.hpp"

class DebugMainFrame : public DebugMainFrameBase
{
public:
	DebugMainFrame(wxWindow* parent);
	virtual ~DebugMainFrame();

	void ClearDebugMessage() { m_richTextCtrlDebugText->Clear(); }
	void AppendDebugMessage(wxString message) { m_richTextCtrlDebugText->AppendText(message); }
};
#endif // DEBUGMAINFRAME_HPP
