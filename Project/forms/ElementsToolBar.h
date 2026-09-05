#ifndef ELEMENTSTOOLBAR_H
#define ELEMENTSTOOLBAR_H

#include <wx/frame.h>
#include <wx/toolbar.h>

class MainFrame;

/**
 * @class ElementsToolBar
 * @brief Floating vertical toolbar for inserting circuit elements.
 */
class ElementsToolBar : public wxFrame
{
public:
	ElementsToolBar(MainFrame* parent);
	virtual ~ElementsToolBar();

	void EnableTools(bool enable);
	wxToolBar* GetToolBar() { return m_toolBar; }

protected:
	void InitToolBar();
	void OnToolClicked(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);

private:
	MainFrame* m_mainFrame = nullptr;
	wxToolBar* m_toolBar = nullptr;
};

#endif // ELEMENTSTOOLBAR_H
