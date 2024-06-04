#include "DebugMainFrame.hpp"

DebugMainFrame::DebugMainFrame(wxWindow* parent)
    : DebugMainFrameBase(parent)
{
    m_richTextCtrlDebugText->Clear();
}

DebugMainFrame::~DebugMainFrame()
{
}

