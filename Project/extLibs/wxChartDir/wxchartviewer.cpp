/*
** Name:        wxchartviewer.cpp
** Purpose:     wxChartViewer defines
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018-2023, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

/// \file wxchartviewer.cpp Implementation of the \b wxChartViewer component

#include "wxchartviewer.h"
#include <time.h>

#include <wx/dcclient.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include <wx/mstream.h>
#include <wx/log.h>
#include <wx/graphics.h>
#include <wx/image.h>
#include <wx/popupwin.h>
#include <wx/sizer.h>
#include <wx/dcgraph.h>
#include <wx/colour.h>

#include <wx/filefn.h>
#include <wx/stdpaths.h>
#include <wx/file.h>

#include <map>
#include <algorithm>

namespace {
	static class wxResourceLoader
	{
	public:
		wxResourceLoader()
		{
			Chart::setResourceLoader(loader);
		}

		static bool loader(const char* id, char* (allocator)(int), char** data, int* len)
		{
			wxPathList resourcePathList;
			resourcePathList.AddEnvList("WXCHARTDIR_RESOURCE_PATH");
			resourcePathList.Add(wxStandardPaths::Get().GetResourcesDir());
			resourcePathList.Add(wxStandardPaths::Get().GetExecutablePath());
			resourcePathList.Add(::wxGetCwd());
			wxString filePath = resourcePathList.FindValidPath(wxString::FromUTF8(id));
			bool found = !filePath.IsEmpty();
			if (found)
			{
				wxFile resourceFile(filePath);
				found = resourceFile.IsOpened() &&
					(0 < (*len = (int)resourceFile.Length())) &&
					(0 != (*data = allocator(*len))) &&
					(wxInvalidOffset != resourceFile.Read(*data, *len));
			}
			if (wxString::FromUTF8(id).IsSameAs("texture"))
			{
				wxLogError("Loader asked for resource 'texture'");
			}
			if (!found)
			{
				wxLogError(wxString("Loader: id not found: ") + wxString::FromUTF8(id));
			}
			return found;
		}
	} wxResourceLoaderRegistration;
}

class wxEnhancedTooltip
{
public:
	wxEnhancedTooltip(wxChartViewer* parent)
	{
		m_parent = parent;
		m_isEnabled = false;
		m_bounds = wxRect(0, 0, 0, 0);
	}

	virtual ~wxEnhancedTooltip()
	{
	}

	void DisplayChart(BaseChart* c, int dpi)
	{
		wxString options = (dpi > 0) ? wxString::Format("+dpi=%d;alpha=1;", dpi) : wxString("alpha=1;");
		c->setOutputOptions(options.ToUTF8());
		MemBlock m = c->makeChart(Chart::BMP);
		wxMemoryInputStream in(m.data, m.len);
		wxBitmap bmp(wxImage(in, wxBITMAP_TYPE_BMP));
		m_toolTipBitmap = bmp;
		m_bounds.SetWidth(bmp.GetWidth());
		m_bounds.SetHeight(bmp.GetHeight());
	}

	void Enable(bool enable)
	{
		m_isEnabled = enable;
		if (!enable)
		{
			m_bounds.SetWidth(0);
			m_bounds.SetHeight(0);
		}
	}

	bool IsEnabled() const
	{
		return m_isEnabled;
	}

	wxSize GetSize() const { return m_toolTipBitmap.GetSize(); }

	wxRect GetBounds() const { return m_bounds; }

	void SetOrigin(int x, int y)
	{
		m_bounds.SetLeft(x);
		m_bounds.SetTop(y);
	}

	void DrawToolTip(wxDC& dc)
	{
		if (m_isEnabled)
		{
			if (m_toolTipBitmap.IsOk())
			{
				dc.DrawBitmap(m_toolTipBitmap, m_bounds.GetLeft(), m_bounds.GetTop());
			}
		}
	}

private:
	bool m_isEnabled;
	wxChartViewer* m_parent;
	wxBitmap m_toolTipBitmap;
	wxRect m_bounds;
};

// High DPI support functions

void
wxChartViewer::setDPI(int dpi)
{
	m_dpi = dpi;
}

int
wxChartViewer::getDPI()
{
	int dpi = m_dpi;
	if (dpi < 0)
	{
		return dpi;
	}
	if (dpi == 0)
	{
		dpi = (int)(96 * GetDPIScaleFactor() + 0.5);
	}
	return (dpi < 24) ? 24 : ((dpi > 384) ? 384 : dpi);
}

double
wxChartViewer::ToImageX(int x)
{
	return x * m_toImageScaleX;
}

double
wxChartViewer::ToImageY(int y)
{
	return y * m_toImageScaleY;
}

int
wxChartViewer::ToDisplayX(double x)
{
	double ret = x / m_toImageScaleX;
	return (int)((ret >= 0) ? ret + 0.5 : (ret - 0.5));
}

int
wxChartViewer::ToDisplayY(double y)
{
	double ret = y / m_toImageScaleY;
	return (int)((ret >= 0) ? ret + 0.5 : (ret - 0.5));
}


/////////////////////////////////////////////////////////////////////////////
//
// Mouse cursors for zooming and scrolling support
//
/////////////////////////////////////////////////////////////////////////////

static class wxAseCursorManager
{
private:
	std::map<int, wxCursor*> cacheZoomInCursor;
	std::map<int, wxCursor*> cacheZoomOutCursor;
	std::map<int, wxCursor*> cacheNoZoomCursor;

	wxCursor& getZoomCursor(std::map<int, wxCursor*>& cache, double scale, int flags)
	{
		int dpi = (int)(96 * scale + 0.5);
		std::map<int, wxCursor*>::const_iterator csr = cache.find(dpi);
		wxCursor* cursor = (csr != cache.end()) ? csr->second : 0;
		if (!cursor)
		{
			DrawArea d;
			d.setSize(32, 32, Chart::Transparent);
			d.circle(15, 15, 7, 7, 0x000000, 0xffffff);
			if (flags & 0x1)
				d.hline(12, 18, 15, 0x000000);
			if (flags & 0x2)
				d.vline(12, 18, 15, 0x000000);
			int x[] = { 21, 28, 26, 19 };
			int y[] = { 19, 26, 28, 21 };
			d.polygon(IntArray(x, 4), IntArray(y, 4), 0x000000, 0x000000);
			char buffer[1024];
			sprintf(buffer, "dpi=%d;", dpi);
			d.setOutputOptions(buffer);
			MemBlock m = d.outPNG();
			wxMemoryInputStream in(m.data, m.len);
			wxImage img(in, wxBITMAP_TYPE_PNG);
			img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, (int)(15 * scale + 0.5));
			img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, (int)(15 * scale + 0.5));
			cursor = new wxCursor(img);
			cache[dpi] = cursor;
		}
		return *cursor;
	}

public:
	wxAseCursorManager()
	{
	}
	~wxAseCursorManager()
	{
		std::for_each(cacheZoomInCursor.begin(), cacheZoomInCursor.end(), [](std::pair<const int, wxCursor*>& entry) { delete entry.second; });
		std::for_each(cacheZoomOutCursor.begin(), cacheZoomOutCursor.end(), [](std::pair<const int, wxCursor*>& entry) { delete entry.second; });
		std::for_each(cacheNoZoomCursor.begin(), cacheNoZoomCursor.end(), [](std::pair<const int, wxCursor*>& entry) { delete entry.second; });
	}
	wxCursor& getZoomInCursor(double scale)
	{
		return getZoomCursor(cacheZoomInCursor, scale, 3);
	}
	wxCursor& getZoomOutCursor(double scale)
	{
		return getZoomCursor(cacheZoomOutCursor, scale, 1);
	}
	wxCursor& getNoZoomCursor(double scale)
	{
		return getZoomCursor(cacheNoZoomCursor, scale, 0);
	}
} cursorManager;


// Utility functions

// ChartDirector uses Chart::chartTime, while wxWidgets uses wxDateTime,
// therefore functions are needed to convert from one to another

// Convert from wxDateTime to chartTime
double
wxChartViewer::ConvertDateTimeToChartTime(wxDateTime dt)
{
	return Chart::chartTime(dt.GetYear(), dt.GetMonth() + 1, dt.GetDay(), dt.GetHour(), dt.GetMinute(), dt.GetSecond()) + dt.GetMillisecond() / 1000.0;
}

// Convert from chartTime to wxDateTime
wxDateTime
wxChartViewer::ConvertChartTimeToDateTime(double ct)
{
	double ymdhms = floor(ct);
	int ms = (int)(floor((ct - ymdhms) * 1000));
	int ymd = Chart::getChartYMD(ymdhms);
	int hms = (int)fmod(ymdhms, 86400);

	wxDateTime date((unsigned short)(ymd % 100), (wxDateTime::Month)((ymd % 10000) / 100 - 1), (unsigned short)(ymd / 10000),
		(unsigned short)(hms / 3600), (unsigned short)((hms % 3600) / 60), (unsigned short)(hms % 60), (unsigned short)ms);
	return date;
}

/*
** wxChartViewer
*/

/// Declaration of chart viewer event wxEVT_CHARTVIEWER_CLICKED
wxDEFINE_EVENT(wxEVT_CHARTVIEWER_CLICKED, wxCommandEvent);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_VIEWPORT_CHANGED
wxDEFINE_EVENT(wxEVT_CHARTVIEWER_VIEWPORT_CHANGED, wxCommandEvent);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSEMOVE_CHART
wxDEFINE_EVENT(wxEVT_CHARTVIEWER_MOUSEMOVE_CHART, wxCommandEvent);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSELEAVE_CHART
wxDEFINE_EVENT(wxEVT_CHARTVIEWER_MOUSELEAVE_CHART, wxCommandEvent);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA
wxDEFINE_EVENT(wxEVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA, wxCommandEvent);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSELEAVE_PLOTAREA
wxDEFINE_EVENT(wxEVT_CHARTVIEWER_MOUSELEAVE_PLOTAREA, wxCommandEvent);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSEWHEEL
wxDEFINE_EVENT(wxEVT_CHARTVIEWER_MOUSEWHEEL, wxCommandEvent);

// Constants used in m_delayChartUpdate
enum { NO_DELAY, NEED_DELAY, NEED_UPDATE };

BEGIN_EVENT_TABLE(wxChartViewer, wxPanel)

EVT_PAINT(wxChartViewer::OnPaint)
EVT_MOTION(wxChartViewer::OnMouseMove)
EVT_LEAVE_WINDOW(wxChartViewer::OnLeaveEvent)
EVT_LEFT_DOWN(wxChartViewer::OnMousePressEvent)
EVT_LEFT_UP(wxChartViewer::OnMouseReleaseEvent)
EVT_MOUSEWHEEL(wxChartViewer::OnWheelEvent)
EVT_MOUSE_CAPTURE_LOST(wxChartViewer::OnMouseCaptureLost)

END_EVENT_TABLE()

wxChartViewer::wxChartViewer(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxPanel(parent, id, pos, size, style, name)
{
	// current chart and hot spot tester
	m_currentChart = 0;
	m_hotSpotTester = 0;
	m_imageMapChart = 0;

	// initialize chart configuration
	m_selectBoxLineColor = wxColour(0, 0, 0);
	m_selectBoxLineWidth = 2;
	m_selectBoxLineStyle = wxPENSTYLE_SOLID;
	m_mouseUsage = Chart::MouseUsageDefault;
	m_zoomDirection = Chart::DirectionHorizontal;
	m_zoomInRatio = 2;
	m_zoomOutRatio = 0.5;
	m_mouseWheelZoomRatio = 1;
	m_scrollDirection = Chart::DirectionHorizontal;
	m_minDragAmount = 5;
	m_updateInterval = 20; /* Original: 20 */

	// current state of the mouse
	m_isOnPlotArea = false;
	m_isPlotAreaMouseDown = false;
	m_isDragScrolling = false;
	m_currentHotSpot = -1;
	m_isClickable = false;
	m_isMouseTracking = false;
	m_isInMouseMove = false;

	// chart update rate support
	m_needUpdateChart = false;
	m_needUpdateImageMap = false;
	m_holdTimerActive = false;
	m_holdTimer.Bind(wxEVT_TIMER, &wxChartViewer::OnHoldTimerEvent, this);
	m_isInViewPortChanged = false;
	m_delayUpdateChart = NO_DELAY;
	m_delayedChart = 0;
	m_lastMouseMove = 0;
	m_delayedMouseEvent = 0;
	m_delayedMouseEventTimer.Bind(wxEVT_TIMER, &wxChartViewer::OnDelayTimerEvent, this);
	m_delayImageMapUpdate = false;

	// track cursor support
	m_autoHideMsg = wxString(wxS(""));
	m_currentMouseX = -1;
	m_currentMouseY = -1;
	m_isInMouseMovePlotArea = false;

	// selection rectangle
	m_rectVisible = false;
	m_previousRect = m_currentRect = wxRect(0, 0, 0, 0);

	m_vpControl = NULL;
	m_reentrantGuard = false;

	// High dpi support
	m_dpi = 0;
	m_toImageScaleX = 1;
	m_toImageScaleY = 1;

	// tooltip
	m_enhancedToolTip = NULL;
	m_cdmlToolTipPrefix = "<*block,bgColor=F0F0F0,edgeColor=808080,margin=5,roundedCorners=3*><*font,color=222222*>";
}

wxChartViewer::~wxChartViewer()
{
	delete m_hotSpotTester;
	delete m_imageMapChart;
	delete m_delayedMouseEvent;
	delete m_delayedChart;
	delete m_enhancedToolTip;
}

void
wxChartViewer::OnPaint(wxPaintEvent& evt)
{
	wxBufferedPaintDC dc(this);
	if (m_chartBitmap.IsOk())
	{
		dc.DrawBitmap(m_chartBitmap, 0, 0);
		if (m_enhancedToolTip) m_enhancedToolTip->DrawToolTip(dc);
		if (m_rectVisible) DrawSelectionRect(dc);
	}
}

void
wxChartViewer::PaintNow(BaseChart* c)
{
	wxClientDC dc(this);
	Render(dc, c);
}

void
wxChartViewer::Render(wxDC& dc, BaseChart* c)
{
	int dpi = getDPI();
	wxString options = (dpi > 0) ? wxString::Format("+dpi=%d;alpha=1;", dpi) : wxString("alpha=1;");
	c->setOutputOptions(options.ToUTF8());
	MemBlock m = c->makeChart(Chart::BMP);
	wxMemoryInputStream in(m.data, m.len);
	wxBitmap bmp(wxImage(in, wxBITMAP_TYPE_BMP));
	m_chartBitmap = bmp;
	dc.DrawBitmap(bmp, 0, 0);
	if (m_enhancedToolTip) m_enhancedToolTip->DrawToolTip(dc);
	//if (m_rectVisible) DrawSelectionRect(dc);

	// High dpi support
	m_toImageScaleX = c->getDrawArea()->getWidth() / (double)bmp.GetWidth();
	m_toImageScaleY = c->getDrawArea()->getHeight() / (double)bmp.GetHeight();
}

/////////////////////////////////////////////////////////////////////////////
// wxChartViewer message handlers

// MouseMove event handler
void wxChartViewer::OnMouseMove(wxMouseEvent& event)
{
	if (!event.LeftIsDown() && m_rectVisible)
	{
		SetRectVisible(false);
	}
	// Enable mouse tracking to detect mouse leave events
	m_isMouseTracking = true;

	// On Windows, mouse events can by-pass the event queue. If there are too many mouse events,
	// the event queue may not get processed, preventing other controls from updating. If two mouse
	// events are less than 10ms apart, there is a risk of too many mouse events. So we repost the
	// mouse event as a timer event that is queued up normally, allowing the queue to get processed.
	// EDIT: event distance reduced from 10ms to 5ms for wxWidgets
	unsigned int timeBetweenMouseMove = (((unsigned int)clock()) - m_lastMouseMove) * 1000 / CLOCKS_PER_SEC;
	if ((m_delayedMouseEvent && (timeBetweenMouseMove < 250)) || (timeBetweenMouseMove < 5))
	{
		if (!m_delayedMouseEvent)
		{
			m_delayedMouseEventTimer.Start(1);
		}
		else
		{
			delete m_delayedMouseEvent;
		}

		m_delayedMouseEvent = (wxMouseEvent*)event.Clone();
	}
	else
	{
		CommitMouseMove(event);
		event.Skip();
	}
	//OnSetCursor();
}

// The method that actually performs MouseMove event processing
void
wxChartViewer::CommitMouseMove(wxMouseEvent& event)
{
	// Remember the mouse coordinates for later use
	m_currentMouseX = event.GetX();
	m_currentMouseY = event.GetY();

	// The chart can be updated more than once during mouse move. For example, it can update due to
	// drag to scroll, and also due to drawing track cursor. So we delay updating the display until
	// all all events has occured.
	m_delayUpdateChart = NEED_DELAY;
	m_isInMouseMove = true;

	// Check if mouse is dragging on the plot area
	m_isOnPlotArea = m_isPlotAreaMouseDown || inPlotArea(ToImageX(event.GetX()), ToImageY(event.GetY()));
	if (m_isPlotAreaMouseDown)
	{
		OnPlotAreaMouseDrag(event);
	}

	// Emit mouseMoveChart
	wxCommandEvent mmcEvent(wxEVT_CHARTVIEWER_MOUSEMOVE_CHART);
	mmcEvent.SetId(GetId());
	mmcEvent.SetEventObject(this);
	HandleWindowEvent(mmcEvent);

	if (inExtendedPlotArea(ToImageX(event.GetX()), ToImageY(event.GetY())))
	{
		// Mouse is in extended plot area, emit mouseMovePlotArea
		m_isInMouseMovePlotArea = true;
		wxCommandEvent mmpEvent(wxEVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA);
		mmpEvent.SetId(GetId());
		mmpEvent.SetEventObject(this);
		HandleWindowEvent(mmpEvent);
	}
	else if (m_isInMouseMovePlotArea)
	{
		// Mouse was in extended plot area, but is not in it now, so emit mouseLeavePlotArea
		m_isInMouseMovePlotArea = false;
		wxCommandEvent mlpEvent(wxEVT_CHARTVIEWER_MOUSELEAVE_PLOTAREA);
		mlpEvent.SetId(GetId());
		mlpEvent.SetEventObject(this);
		HandleWindowEvent(mlpEvent);
		ApplyAutoHide(wxS("mouseleaveplotarea"));
	}

	// Show hot spot tool tips if necessary
	if (m_delayImageMapUpdate)
	{
		m_delayImageMapUpdate = false;
		if (!m_isPlotAreaMouseDown)
		{
			updateViewPort(false, true);
		}
	}

	wxPoint cursorPosition(wxPoint(event.GetX(), event.GetY()));

	if (event.ButtonIsDown(wxMOUSE_BTN_ANY))
	{
		// Hide tool tips if mouse button is pressed.
		UnsetToolTip();
	}

	// Use the ChartDirector ImageMapHandler to determine if the mouse is over a hot spot
	int hotSpotNo = 0;
	if (0 != m_hotSpotTester)
	{
		hotSpotNo = m_hotSpotTester->getHotSpot(ToImageX(event.GetX()), ToImageY(event.GetY()), m_imageMapChart);
	}

	const char* isDynamic = 0;
	if (hotSpotNo != 0)
	{
		isDynamic = m_hotSpotTester->getValue("dynamic");
	}

	// If the mouse is in the same hot spot since the last mouse move event, there is no need
	// to update the tool tip.
	if ((hotSpotNo != m_currentHotSpot) || ((0 != isDynamic) && (0 != *isDynamic)))
	{
		// Hot spot has changed - update tool tip text
		m_currentHotSpot = hotSpotNo;

		if (hotSpotNo == 0)
		{
			// Mouse is not actually on handler hot spot - use default tool tip text and reset
			// the clickable flag.
			m_isClickable = false;
			ShowToolTip(m_defaultToolTip, cursorPosition);
		}
		else
		{
			// Mouse is on a hot spot. In this implementation, we consider the hot spot as
			// clickable if its href ("path") parameter is not empty.
			const char* path = m_hotSpotTester->getValue("path");
			m_isClickable = ((0 != path) && (0 != *path));
			ShowToolTip(wxString::FromUTF8(m_hotSpotTester->getValue("title")), cursorPosition);
		}
	}

	if (m_enhancedToolTip && m_enhancedToolTip->IsEnabled())
	{
		MoveEnhancedToolTip(cursorPosition, m_enhancedToolTip->GetBounds());
	}

	// Cancel the delayed mouse event if any
	if (m_delayedMouseEvent)
	{
		m_delayedMouseEventTimer.Stop();
		delete m_delayedMouseEvent;
		m_delayedMouseEvent = NULL;
	}

	// Can update chart now
	CommitUpdateChart();
	m_isInMouseMove = false;

	m_lastMouseMove = (unsigned int)clock();
}

void
wxChartViewer::ShowToolTip(const wxString& text, const wxPoint& cursorPosition)
{
	if ((text.length() <= 0) || ((text[0] == '<') && (text[1] == '*')))
	{
		ShowEnhancedToolTip(text, cursorPosition);
		UnsetToolTip();
	}
	else
	{
		ShowEnhancedToolTip("", cursorPosition);
		SetToolTip(text);
	}
}

void
wxChartViewer::ShowEnhancedToolTip(const wxString& text, const wxPoint& cursorPosition)
{
	if (!m_enhancedToolTip)
	{
		m_enhancedToolTip = new wxEnhancedTooltip(this);
	}

	if (text.IsEmpty())
	{
		if (m_enhancedToolTip->IsEnabled())
		{
			wxRect current = m_enhancedToolTip->GetBounds();
			m_enhancedToolTip->Enable(false);
			Refresh(false, &current);
		}
		return;
	}

	wxString buffer = text;
	buffer.Replace(wxString("<*cdml*>"), m_cdmlToolTipPrefix);

	wxRect rFrom = m_enhancedToolTip->GetBounds();
	PieChart c(1, 1, Chart::Transparent);
	c.makeChart()->renderCDML(buffer.ToUTF8());
	m_enhancedToolTip->DisplayChart(&c, getDPI());
	m_enhancedToolTip->Enable(true);

	MoveEnhancedToolTip(cursorPosition, rFrom);
}

void
wxChartViewer::MoveEnhancedToolTip(const wxPoint& p, const wxRect& rFrom)
{
	int x = p.x;
	// Magic number 24 - what is it?
	int y = p.y + 24;

	wxSize viewerSize = GetSize();
	wxSize toolTipSize = m_enhancedToolTip->GetSize();

	if (x + toolTipSize.GetWidth() > viewerSize.GetWidth())
	{
		x = viewerSize.GetWidth() - toolTipSize.GetWidth();
		if (x < 0) x = 0;
	}
	if (y + toolTipSize.GetHeight() > viewerSize.GetHeight())
	{
		y = p.y - toolTipSize.GetHeight() - 2;
		if (y < 0) y = 0;
	}

	// Move tooltip to new position and refresh
	m_enhancedToolTip->SetOrigin(x, y);
	wxRect rCurrent = m_enhancedToolTip->GetBounds();
	if (rFrom.GetWidth() > 0)
	{
		rCurrent.Union(rFrom);
	}
	Refresh(false, &rCurrent);
}

// Delayed MouseMove event handler
void
wxChartViewer::OnDelayedMouseMove()
{
	if (m_delayedMouseEvent)
	{
		CommitMouseMove(*m_delayedMouseEvent);
	}
}

void
wxChartViewer::OnLeaveEvent(wxMouseEvent& event)
{
	// Process delayed mouse move, if any
	OnDelayedMouseMove();

	// Mouse tracking is no longer active
	m_isMouseTracking = false;

	if (m_isInMouseMovePlotArea)
	{
		// Mouse was in extended plot area, but is not in it now, so emit mouseLeavePlotArea
		m_isInMouseMovePlotArea = false;
		wxCommandEvent mlpEvent(wxEVT_CHARTVIEWER_MOUSELEAVE_PLOTAREA);
		mlpEvent.SetId(GetId());
		mlpEvent.SetEventObject(this);
		HandleWindowEvent(mlpEvent);
		ApplyAutoHide(wxS("mouseleaveplotarea"));
	}

	// emit mouseLeaveChart
	wxCommandEvent mlcEvent(wxEVT_CHARTVIEWER_MOUSELEAVE_CHART);
	mlcEvent.SetId(GetId());
	mlcEvent.SetEventObject(this);
	HandleWindowEvent(mlcEvent);
	ApplyAutoHide(wxS("mouseleavechart"));

	// hide tooltip
	ShowEnhancedToolTip("");

	event.Skip();
}

// MouseWheel handler
void
wxChartViewer::OnWheelEvent(wxMouseEvent& event)
{
	// Process delayed mouse move, if any
	OnDelayedMouseMove();

	// emit mouseWheel event
	wxCommandEvent mwEvent(wxEVT_CHARTVIEWER_MOUSEWHEEL);
	mwEvent.SetId(GetId());
	mwEvent.SetEventObject(this);
	mwEvent.SetInt(event.GetWheelRotation());
	HandleWindowEvent(mwEvent);

	// Process the mouse wheel only if the mouse is over the plot area
	bool hasMouseWheelZoom = isMouseOnPlotArea() &&
		onMouseWheelZoom(getPlotAreaMouseX(), getPlotAreaMouseY(), event.GetWheelRotation());
	event.Skip();
}

bool
wxChartViewer::onMouseWheelZoom(int x, int y, int zDelta)
{
	// Zoom ratio = 1 means no zooming
	if (m_mouseWheelZoomRatio == 1)
	{
		return false;
	}

	// X and Y zoom ratios
	double rx = 1;
	double ry = 1;
	if (getZoomDirection() != Chart::DirectionVertical)
	{
		rx = (zDelta > 0) ? m_mouseWheelZoomRatio : 1 / m_mouseWheelZoomRatio;
	}
	if (getZoomDirection() != Chart::DirectionHorizontal)
	{
		ry = (zDelta > 0) ? m_mouseWheelZoomRatio : 1 / m_mouseWheelZoomRatio;
	}

	// Do the zooming
	if (zoomAround(x, y, rx, ry))
	{
		updateViewPort(true, true);
	}

	return true;
}

// Change the mouse cursor.
void
wxChartViewer::OnSetCursor()
{
	if (m_isDragScrolling)
	{
		switch (m_scrollDirection)
		{
		case Chart::DirectionHorizontal:
			SetCursor(wxCursor(wxCURSOR_SIZEWE));
			break;
		case Chart::DirectionVertical:
			SetCursor(wxCursor(wxCURSOR_SIZENS));
			break;
		default:
			SetCursor(wxCursor(wxCURSOR_SIZING));
			break;
		}
		return;
	}

	if (m_isOnPlotArea)
	{
		switch (m_mouseUsage)
		{
		case Chart::MouseUsageZoomIn:
			if (canZoomIn(m_zoomDirection))
			{
				//SetCursor(cursorManager.getZoomInCursor(GetContentScaleFactor()));
			}
			else
			{
				//SetCursor(cursorManager.getNoZoomCursor(GetContentScaleFactor()));
			}
			return;
		case Chart::MouseUsageZoomOut:
			if (canZoomOut(m_zoomDirection))
			{
				//SetCursor(cursorManager.getZoomOutCursor(GetContentScaleFactor()));
			}
			else
			{
				//SetCursor(cursorManager.getNoZoomCursor(GetContentScaleFactor()));
			}
			return;
		}
	}

	if (m_isClickable)
	{
		SetCursor(wxCursor(wxCURSOR_HAND));
	}
	else
	{
		SetCursor(wxCursor(wxCURSOR_ARROW));
	}
}

// Mouse capture lost event handler
void
wxChartViewer::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
	// TODO: Any action required?
	// wxLogDebug("*** OnMouseCaptureLost ***");
}

// Mouse button down event.
void
wxChartViewer::OnMousePressEvent(wxMouseEvent& event)
{
	OnDelayedMouseMove();

	if (inPlotArea(ToImageX(event.GetX()), ToImageY(event.GetY())) && (m_mouseUsage != Chart::MouseUsageDefault))
	{
		// Mouse usage is for drag to zoom/scroll. Capture the mouse to prepare for dragging and
		// save the mouse down position to draw the selection rectangle.
		if (!HasCapture())
		{
			CaptureMouse();
		}
		m_isPlotAreaMouseDown = true;
		m_plotAreaMouseDownXPos = event.GetX();
		m_plotAreaMouseDownYPos = event.GetY();
		startDrag();
	}
	event.Skip();
}

// Mouse button up event.
void
wxChartViewer::OnMouseReleaseEvent(wxMouseEvent& event)
{
	OnDelayedMouseMove();
	if (m_isPlotAreaMouseDown)
	{
		// Release the mouse capture.
		if (HasCapture())
		{
			ReleaseMouse();
		}
		m_isPlotAreaMouseDown = false;
		SetRectVisible(false);
		bool hasUpdate = false;

		switch (m_mouseUsage)
		{
		case Chart::MouseUsageZoomIn:
			if (canZoomIn(m_zoomDirection))
			{
				if (IsDrag(m_zoomDirection, event))
				{
					int minX, minY, spanX, spanY;
					GetDragZoomRect(event.GetX(), event.GetY(), minX, minY, spanX, spanY);

					// Zoom to the drag selection rectangle.
					hasUpdate = zoomTo(m_zoomDirection, ToImageX(minX), ToImageY(minY), ToImageX(minX + spanX), ToImageY(minY + spanY));
				}
				else
				{
					// User just click on a point. Zoom-in around the mouse cursor position.
					hasUpdate = zoomAt(m_zoomDirection, ToImageX(event.GetX()), ToImageY(event.GetY()), m_zoomInRatio);
				}
			}
			break;
		case Chart::MouseUsageZoomOut:
			// Zoom out around the mouse cursor position.
			if (canZoomOut(m_zoomDirection))
			{
				hasUpdate = zoomAt(m_zoomDirection, ToImageX(event.GetX()), ToImageY(event.GetY()), m_zoomOutRatio);
			}
			break;
		default:
			if (m_isDragScrolling)
			{
				// Drag to scroll. We can update the image map now as scrolling has finished.
				updateViewPort(false, true);
			}
			else
			{
				// Is not zooming or scrolling, so is just a normal click event.
				wxCommandEvent clickEvent(wxEVT_CHARTVIEWER_CLICKED);
				clickEvent.SetId(GetId());
				clickEvent.SetEventObject(this);
				HandleWindowEvent(clickEvent);
			}
			break;
		}

		m_isDragScrolling = false;
		if (hasUpdate)
		{
			// View port has changed - update it.
			updateViewPort(true, true);
		}
	}
	else
	{
		if (m_rectVisible)
		{
			SetRectVisible(false);
		}
		wxCommandEvent clickEvent(wxEVT_CHARTVIEWER_CLICKED);
		clickEvent.SetId(GetId());
		clickEvent.SetEventObject(this);
		HandleWindowEvent(clickEvent);
	}

	//OnSetCursor();
	event.Skip();
}

// Chart hold timer.
void
wxChartViewer::OnHoldTimerEvent(wxTimerEvent& event)
{
	if (m_holdTimerActive)
	{
		m_holdTimer.Stop();
		m_holdTimerActive = false;

		// If has pending chart update request, handles them now.
		if (m_needUpdateChart || m_needUpdateImageMap)
		{
			updateViewPort(m_needUpdateChart, m_needUpdateImageMap);
		}
	}
}

// Chart hold timer.
void
wxChartViewer::OnDelayTimerEvent(wxTimerEvent& event)
{
	if (m_delayedMouseEvent != NULL)
	{
		// Is a delayed mouse move event
		OnDelayedMouseMove();
	}
}

////////////////////////////////////////////////////////////////////////
// wxChartViewer properties

// Set the chart to the control
void
wxChartViewer::setChart(BaseChart* c)
{
	if (m_currentChart != c)
	{
		setImageMap(0);
	}

	m_currentChart = c;
	delete m_imageMapChart;
	m_imageMapChart = c ? new BaseChart(c) : 0;

	if (0 != c)
	{
		commitPendingSyncAxis(c);
		if (m_delayUpdateChart != NO_DELAY)
		{
			c->makeChart();
		}
	}

	updateDisplay();
}

// Get back the same BaseChart pointer provided by the previous setChart call.
BaseChart*
wxChartViewer::getChart()
{
	return m_currentChart;
}

// Set image map used by the chart
void
wxChartViewer::setImageMap(const char* imageMap)
{
	// Delete the existing ImageMapHandler
	if (0 != m_hotSpotTester)
	{
		delete m_hotSpotTester;
	}
	m_currentHotSpot = -1;
	m_isClickable = false;

	// Create a new ImageMapHandler to represent the image map
	if ((0 == imageMap) || (0 == *imageMap))
	{
		m_hotSpotTester = 0;
	}
	else
	{
		m_hotSpotTester = new ImageMapHandler(imageMap, this);
	}
}

// Get the image map handler for the chart
ImageMapHandler*
wxChartViewer::getImageMapHandler()
{
	return m_hotSpotTester;
}

// Set the default tool tip to use
void
wxChartViewer::setDefaultToolTip(const wxString& text)
{
	m_defaultToolTip = text;
}

// Set the CDML tool tip prefix
void
wxChartViewer::setCDMLToolTipPrefix(const wxString& prefix)
{
	m_cdmlToolTipPrefix = prefix;
}

// Set the wxViewPortControl to be associated with this control
void
wxChartViewer::setViewPortControl(wxViewPortControl* vpc)
{
	if (m_reentrantGuard)
	{
		return;
	}
	m_reentrantGuard = true;

	if (0 != m_vpControl)
	{
		m_vpControl->setViewer(0);
	}
	m_vpControl = vpc;
	if (0 != m_vpControl)
	{
		m_vpControl->setViewer(this);
	}

	m_reentrantGuard = false;
}

// Get the wxViewPortControl that is associated with this control
wxViewPortControl*
wxChartViewer::getViewPortControl()
{
	return m_vpControl;
}

// Set the border width of the selection box
void
wxChartViewer::setSelectionBorderWidth(int width)
{
	m_selectBoxLineWidth = width;
}

// Get the border with of the selection box.
int
wxChartViewer::getSelectionBorderWidth()
{
	return m_selectBoxLineWidth;
}

// Set the border color of the selection box
void
wxChartViewer::setSelectionBorderColor(const wxColour& color)
{
	m_selectBoxLineColor = color;
}

// Get the border color of the selection box.
wxColour
wxChartViewer::getSelectionBorderColor()
{
	return m_selectBoxLineColor;
}

// Set the border style of the selection box
void
wxChartViewer::setSelectionBorderStyle(wxPenStyle style)
{
	m_selectBoxLineStyle = style;
}

// Get the border style of the selection box
wxPenStyle
wxChartViewer::getSelectionBorderStyle()
{
	return m_selectBoxLineStyle;
}

// Set the mouse usage mode
void
wxChartViewer::setMouseUsage(int mouseUsage)
{
	m_mouseUsage = mouseUsage;
}

// Get the mouse usage mode
int
wxChartViewer::getMouseUsage()
{
	return m_mouseUsage;
}

// Set the zoom direction
void
wxChartViewer::setZoomDirection(int direction)
{
	m_zoomDirection = direction;
}

// Get the zoom direction
int
wxChartViewer::getZoomDirection()
{
	return m_zoomDirection;
}

// Set the scroll direction
void
wxChartViewer::setScrollDirection(int direction)
{
	m_scrollDirection = direction;
}

// Get the scroll direction
int
wxChartViewer::getScrollDirection()
{
	return m_scrollDirection;
}

// Set the zoom-in ratio for mouse click zoom-in
void
wxChartViewer::setZoomInRatio(double ratio)
{
	m_zoomInRatio = ratio;
}

// Get the zoom-in ratio for mouse click zoom-in
double
wxChartViewer::getZoomInRatio()
{
	return m_zoomInRatio;
}

// Set the zoom-out ratio
void
wxChartViewer::setZoomOutRatio(double ratio)
{
	m_zoomOutRatio = ratio;
}

// Get the zoom-out ratio
double
wxChartViewer::getZoomOutRatio()
{
	return m_zoomOutRatio;
}

// Set the mouse wheel zoom ratio 
void
wxChartViewer::setMouseWheelZoomRatio(double ratio)
{
	m_mouseWheelZoomRatio = ratio;
}

// Get the mouse wheel zoom ratio 
double
wxChartViewer::getMouseWheelZoomRatio()
{
	return m_mouseWheelZoomRatio;
}

// Set the minimum mouse drag before the dragging is considered as real. This is to avoid small
// mouse vibrations triggering a mouse drag.
void
wxChartViewer::setMinimumDrag(int offset)
{
	m_minDragAmount = offset;
}

// Get the minimum mouse drag before the dragging is considered as real.
int
wxChartViewer::getMinimumDrag()
{
	return m_minDragAmount;
}

// Set the minimum interval between ViewPortChanged events. This is to avoid the chart being
// updated too frequently. (Default is 20ms between chart updates.) Multiple update events
// arrived during the interval will be merged into one chart update and executed at the end
// of the interval.
void
wxChartViewer::setUpdateInterval(int interval)
{
	m_updateInterval = interval;
}

// Get the minimum interval between ViewPortChanged events.
int
wxChartViewer::getUpdateInterval()
{
	return m_updateInterval;
}

// Check if there is a pending chart update request.
bool
wxChartViewer::needUpdateChart()
{
	return m_needUpdateChart;
}

// Check if there is a pending image map update request.
bool
wxChartViewer::needUpdateImageMap()
{
	return m_needUpdateImageMap;
}

// Get the current mouse x coordinate when used in a mouse move event handler
int
wxChartViewer::getChartMouseX()
{
	return (m_currentMouseX < -0x10000000) ? (getPlotAreaLeft() + getPlotAreaWidth())
		: (int)(ToImageX(m_currentMouseX) + 0.5);
}

// Get the current mouse y coordinate when used in a mouse move event handler
int
wxChartViewer::getChartMouseY()
{
	return (m_currentMouseY < -0x10000000) ? (getPlotAreaTop() + getPlotAreaHeight())
		: (int)(ToImageY(m_currentMouseY) + 0.5);
}

// Get the current mouse x coordinate bounded to the plot area when used in a mouse event handler
int
wxChartViewer::getPlotAreaMouseX()
{
	int ret = getChartMouseX();
	if (ret < getPlotAreaLeft())
	{
		ret = getPlotAreaLeft();
	}
	if (ret > getPlotAreaLeft() + getPlotAreaWidth())
	{
		ret = getPlotAreaLeft() + getPlotAreaWidth();
	}
	return ret;
}

// Get the current mouse y coordinate bounded to the plot area when used in a mouse event handler
int
wxChartViewer::getPlotAreaMouseY()
{
	int ret = getChartMouseY();
	if (ret < getPlotAreaTop())
	{
		ret = getPlotAreaTop();
	}
	if (ret > getPlotAreaTop() + getPlotAreaHeight())
	{
		ret = getPlotAreaTop() + getPlotAreaHeight();
	}
	return ret;
}

// Check if mouse is on the extended plot area
bool
wxChartViewer::isMouseOnPlotArea()
{
	return ((m_isMouseTracking) ? inExtendedPlotArea(getChartMouseX(), getChartMouseY()) : false);
}

// Check if mouse is dragging to scroll or to select the zoom rectangle
bool
wxChartViewer::isMouseDragging()
{
	return m_isPlotAreaMouseDown;
}

// Check if is currently processing a mouse move event
bool
wxChartViewer::isInMouseMoveEvent()
{
	return m_isInMouseMove;
}

// Check if is currently processing a view port changed event
bool
wxChartViewer::isInViewPortChangedEvent()
{
	return m_isInViewPortChanged;
}

////////////////////////////////////////////////////////////////////////
// wxChartViewer methods

// Update the display
void
wxChartViewer::updateDisplay()
{
	if (m_delayUpdateChart == NO_DELAY)
	{
		CommitUpdateChart();
	}
	else
	{
		m_delayUpdateChart = NEED_UPDATE;
		delete m_delayedChart;
		m_delayedChart = (0 != m_currentChart) ? new BaseChart(m_currentChart) : 0;
	}
}

// Commit chart to display
void
wxChartViewer::CommitUpdateChart()
{
	if (m_delayUpdateChart == NEED_DELAY)
	{
		// No actual update occur
		m_delayUpdateChart = NO_DELAY;
		return;
	}

	// Get the image and metrics for the chart
	BaseChart* c = (m_delayUpdateChart == NEED_UPDATE) ? m_delayedChart : m_currentChart;
	const char* chartMetrics = 0;

	if (c)
	{
		PaintNow(c);
		SetMinSize(wxSize(ToDisplayX(c->getDrawArea()->getWidth()), ToDisplayY(c->getDrawArea()->getHeight())));

		// Get chart metrics
		chartMetrics = c->getChartMetrics();
	}

	// Set the chart metrics and clear the image map
	setChartMetrics(chartMetrics);

	// Any delayed chart has been committed
	m_delayUpdateChart = NO_DELAY;
	delete m_delayedChart;
	m_delayedChart = 0;
}

// Set the message used to remove the dynamic layer
void
wxChartViewer::removeDynamicLayer(const wxString& msg)
{
	m_autoHideMsg = msg.Lower();
	if (m_autoHideMsg.IsSameAs(wxS("now")))
	{
		ApplyAutoHide(msg);
	}
}

// Attempt to hide the dynamic layer using the specified message
void
wxChartViewer::ApplyAutoHide(const wxString& msg)
{
	if (m_autoHideMsg == msg)
	{
		if (0 != m_currentChart)
		{
			m_currentChart->removeDynamicLayer();
		}
		m_autoHideMsg = wxEmptyString;
		updateDisplay();
	}
}

// Set selection rectangle position and size
void
wxChartViewer::SetSelectionRect(int x, int y, int width, int height)
{
	// width < 0 is interpreted as the rectangle extends to the left or x.
	// height < 0 is interpreted as the rectangle extends to above y.
	if (width < 0)
	{
		x -= (width = -width);
	}
	if (height < 0)
	{
		y -= (height = -height);
	}
	m_previousRect = m_currentRect;
	m_currentRect = wxRect(x, y, width - 1, height - 1);
}

// Show/hide selection rectangle
void
wxChartViewer::SetRectVisible(bool visible)
{
	m_rectVisible = visible;

	if (m_currentRect.GetWidth() > 0 && m_currentRect.GetHeight() > 0)
	{
		wxRect r = m_currentRect;
		if (m_previousRect.GetWidth() > 0)
		{
			r.Union(m_previousRect);
		}
		if ((r.x -= 5) < 0) r.x = 0;
		if ((r.y -= 5) < 0) r.y = 0;
		r.width += 10;
		r.height += 10;
		Refresh(false, &r);
	}

	if (!m_rectVisible)
	{
		m_currentRect = wxRect(0, 0, 0, 0);
	}
}

void
wxChartViewer::DrawSelectionRect(wxBufferedDC& dc)
{
	if (m_currentRect.GetWidth() > 0 && m_currentRect.GetHeight() > 0)
	{
		if (m_rectVisible)
		{
			wxRect rect(m_currentRect.x, m_currentRect.y, m_currentRect.width, m_currentRect.height);
			wxPen pen(m_selectBoxLineColor, m_selectBoxLineWidth, m_selectBoxLineStyle);
			//pen.SetCap(wxCAP_PROJECTING);
			//
			//dc.SetPen(pen);
			//dc.SetBrush(*wxTRANSPARENT_BRUSH);
			//dc.DrawRectangle(rect);
			wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
			if (gc) {
				gc->SetPen(pen);
				gc->SetBrush(wxBrush(wxColour(m_selectBoxLineColor.GetRed(), m_selectBoxLineColor.GetGreen(), m_selectBoxLineColor.GetBlue(), 126)));
				gc->DrawRectangle(rect.x, rect.y, rect.width, rect.height);
				delete gc;
			}
		}
	}
}

// Determines if the mouse is dragging.
bool
wxChartViewer::IsDrag(int direction, wxMouseEvent& event)
{
	// We only consider the mouse is dragging if it has dragged more than m_minDragAmount.
	// This is to avoid small mouse vibrations triggering a mouse drag.
	int spanX = abs(event.GetX() - m_plotAreaMouseDownXPos);
	int spanY = abs(event.GetY() - m_plotAreaMouseDownYPos);
	return ((direction != Chart::DirectionVertical) && (spanX >= m_minDragAmount)) ||
		((direction != Chart::DirectionHorizontal) && (spanY >= m_minDragAmount));
}

void
wxChartViewer::GetDragZoomRect(int px, int py, int& x, int& y, int& w, int& h)
{
	x = (px < m_plotAreaMouseDownXPos) ? px : m_plotAreaMouseDownXPos;
	y = (py < m_plotAreaMouseDownYPos) ? py : m_plotAreaMouseDownYPos;
	w = abs(px - m_plotAreaMouseDownXPos);
	h = abs(py - m_plotAreaMouseDownYPos);

	if (getZoomXYRatio() > 0)
	{
		double imageXYRatio = getPlotAreaWidth() / (double)getPlotAreaHeight();

		double whDelta = ToImageX(w) - ToImageY(h) * imageXYRatio;
		if (whDelta < 0)
		{
			w = ToDisplayX(ToImageY(h) * imageXYRatio);
		}
		else if (whDelta > 0)
		{
			h = ToDisplayY(ToImageX(w) / imageXYRatio);
		}

		if (x == px)
		{
			x = m_plotAreaMouseDownXPos - w;
		}
		if (y == py)
		{
			y = m_plotAreaMouseDownYPos - h;
		}
	}
}

// Process mouse dragging over the plot area
void
wxChartViewer::OnPlotAreaMouseDrag(wxMouseEvent& event)
{
	switch (m_mouseUsage)
	{
	case Chart::MouseUsageZoomIn:
	{
		// Mouse is used for zoom in. Draw the selection rectangle if necessary.
		bool isDragZoom = event.LeftIsDown() && canZoomIn(m_zoomDirection) && IsDrag(m_zoomDirection, event);
		if (isDragZoom)
		{
			int minX, minY, spanX, spanY;
			GetDragZoomRect(event.GetX(), event.GetY(), minX, minY, spanX, spanY);
			switch (m_zoomDirection)
			{
			case Chart::DirectionHorizontal:
				SetSelectionRect(minX, ToDisplayY(getPlotAreaTop()), spanX, ToDisplayY(getPlotAreaHeight()));
				break;
			case Chart::DirectionVertical:
				SetSelectionRect(ToDisplayX(getPlotAreaLeft()), minY, ToDisplayX(getPlotAreaWidth()), spanY);
				break;
			default:
				SetSelectionRect(minX, minY, spanX, spanY);
				break;
			}
		}
		SetRectVisible(isDragZoom);
		break;
	}
	case Chart::MouseUsageScroll:
	{
		// Mouse is used for drag scrolling. Scroll and update the view port.
		if (m_isDragScrolling || IsDrag(m_scrollDirection, event))
		{
			m_isDragScrolling = true;
			if (dragTo(m_scrollDirection, ToImageX(event.GetX() - m_plotAreaMouseDownXPos), ToImageY(event.GetY() - m_plotAreaMouseDownYPos)))
			{
				updateViewPort(true, false);
			}
		}
	}
	}
}

// Trigger the ViewPortChanged event
void
wxChartViewer::updateViewPort(bool needUpdateChart, bool needUpdateImageMap)
{
	// Already updating, no need to update again
	if (m_isInViewPortChanged)
	{
		return;
	}

	// Merge the current update requests with any pending requests.
	m_needUpdateChart = m_needUpdateChart || needUpdateChart;
	m_needUpdateImageMap = needUpdateImageMap;

	// Hold timer has not expired, so do not update chart immediately. Keep the requests pending.
	if (m_holdTimerActive)
	{
		return;
	}

	// The chart can be updated more than once during mouse move. For example, it can update due to
	// drag to scroll, and also due to drawing track cursor. So we delay updating the display until
	// all all updates has occured.
	int hasDelayUpdate = (m_delayUpdateChart != NO_DELAY);
	if (!hasDelayUpdate)
	{
		m_delayUpdateChart = NEED_DELAY;
	}

	// Can trigger the ViewPortChanged event.
	validateViewPort();
	m_isInViewPortChanged = true;
	wxCommandEvent vpcEvent(wxEVT_CHARTVIEWER_VIEWPORT_CHANGED);
	vpcEvent.SetId(GetId());
	vpcEvent.SetEventObject(this);
	HandleWindowEvent(vpcEvent);

	m_isInViewPortChanged = false;

	if (m_needUpdateChart && (0 != m_vpControl))
	{
		m_vpControl->updateDisplay();
	}

	// Can update chart now
	if (!hasDelayUpdate)
	{
		CommitUpdateChart();
	}

	// Clear any pending updates.
	m_needUpdateChart = false;
	m_needUpdateImageMap = false;

	// Set hold timer to prevent multiple chart updates within a short period.
	if (m_updateInterval > 0)
	{
		m_holdTimerActive = true;
		m_holdTimer.Start(m_updateInterval);
	}
}

////////////////////////////////////////////////////////////////////////
//
// wxViewPortControl
//
////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxViewPortControl, wxPanel)

EVT_PAINT(wxViewPortControl::OnPaint)
EVT_MOTION(wxViewPortControl::OnMouseMoveEvent)
//EVT_LEAVE_WINDOW(wxViewPortControl::OnLeaveEvent)
EVT_LEFT_DOWN(wxViewPortControl::OnMousePressEvent)
EVT_LEFT_UP(wxViewPortControl::OnMouseReleaseEvent)
EVT_MOUSEWHEEL(wxViewPortControl::OnMouseWheelEvent)
EVT_MOUSE_CAPTURE_LOST(wxViewPortControl::OnMouseCaptureLost)

END_EVENT_TABLE()

wxViewPortControl::wxViewPortControl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxPanel(parent, id, pos, size, style, name)
{
	m_chartViewer = 0;
	m_chart = 0;
	m_reentrantGuard = false;

	m_mouseDownX = 0;
	m_mouseDownY = 0;
	m_isPlotAreaMouseDown = false;
}

// Free	resources
wxViewPortControl::~wxViewPortControl()
{
}

// Set the wxChartViewer to be associated with this control
void
wxViewPortControl::setViewer(wxChartViewer* viewer)
{
	if (m_reentrantGuard)
	{
		return;
	}

	m_reentrantGuard = true;
	if (m_chartViewer)
	{
		m_chartViewer->setViewPortControl(0);
	}
	m_chartViewer = viewer;
	setViewPortManager(viewer);
	if (m_chartViewer)
	{
		m_chartViewer->setViewPortControl(this);
	}
	m_reentrantGuard = false;

	updateDisplay();
}

// Get back	the	same wxChartViewer pointer provided	by the previous	setViewer call.
wxChartViewer*
wxViewPortControl::getViewer()
{
	return m_chartViewer;
}

// Set the chart to	be displayed in the	control
void
wxViewPortControl::setChart(BaseChart* c)
{
	m_chart = c;
	ViewPortControlBase::setChart(c);
	updateDisplay();
}

// Get back	the	same BaseChart pointer provided	by the previous	setChart call.
BaseChart*
wxViewPortControl::getChart()
{
	return m_chart;
}

// Convert from mouse x-coordinate to image x-coordinate
double
wxViewPortControl::ToImageX(int x)
{
	// In this version, no conversion is done. It is assumed the control does not stretch or shrink
	// the image and does not provide any additional margin to offset the image.
	return x;
}

// Convert from mouse x-coordinate to image x-coordinate
double
wxViewPortControl::ToImageY(int y)
{
	// In this version, no conversion is done. It is assumed the control does not stretch or shrink
	// the image and does not provide any additional margin to offset the image.
	return y;
}

void
wxViewPortControl::OnPaint(wxPaintEvent& evt)
{
	wxPaintDC dc(this);
	BaseChart* c = m_chart;
	if (c)
	{
		Render(dc, c);
	}
}

void
wxViewPortControl::PaintNow(BaseChart* c)
{
	if (c)
	{
		wxClientDC dc(this);
		Render(dc, c);
	}
}

void
wxViewPortControl::Render(wxDC& dc, BaseChart* c)
{
#if 0
	int dpi = (m_chartViewer) ? m_chartViewer->getDPI() : 0;
	wxString options = (dpi > 0) ? wxString::Format("+dpi=%d;alpha=1;", dpi) : wxString("alpha=1;");
	c->setOutputOptions(options.ToUTF8());
#endif
	MemBlock m = c->makeChart(Chart::BMP);
	wxMemoryInputStream in(m.data, m.len);
	wxBitmap bmp(wxImage(in, wxBITMAP_TYPE_BMP));
	dc.DrawBitmap(bmp, 0, 0);
}

// Display the chart
void
wxViewPortControl::PaintDisplay()
{
	BaseChart* c = getChart();
	if (c != NULL)
	{
		PaintNow(c);
		SetMinSize(wxSize(c->getWidth(), c->getHeight()));
	}
}

// Handle the ViewPortChanged event from the associated wxChartViewer
void
wxViewPortControl::OnViewPortChanged()
{
	updateDisplay();
}

// Mouse capture lost event handler
void
wxViewPortControl::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
	// TODO: Any action required?
}

// Mouse button down event.
void
wxViewPortControl::OnMousePressEvent(wxMouseEvent& event)
{
	event.Skip();
	if (!event.ButtonIsDown(wxMOUSE_BTN_ANY))
	{
		return;
	}

	// Enable mouse capture for drag support
	if (!HasCapture())
	{
		CaptureMouse();
	}

	// Remember current mouse position
	m_mouseDownX = event.GetX();
	m_mouseDownY = event.GetY();
	m_isPlotAreaMouseDown = true;

	// Get the wxChartViewer zoom/scroll state to determine which type of mouse action is allowed
	SyncState();

	// Handle the mouse down event
	handleMouseDown(ToImageX(event.GetX()), ToImageY(event.GetY()));

	// Update the chart viewer if the viewport has changed
	UpdateChartViewerIfNecessary();
	// event.Skip();
}

// MouseMove event handler
void
wxViewPortControl::OnMouseMoveEvent(wxMouseEvent& event)
{
	//if (m_isPlotAreaMouseDown)
	{
		// Get the wxChartViewer zoom/scroll state to determine which type of mouse action is allowed
		SyncState();

		// Handle the mouse move event
		handleMouseMove(ToImageX(event.GetX()), ToImageY(event.GetY()), IsDrag(event));

		// Update the chart viewer if the viewport has changed
		UpdateChartViewerIfNecessary();
	}

	// Update the mouse cursor
	//UpdateCursor(getCursor());

	// Update the display
	if (needUpdateDisplay())
	{
		PaintDisplay();
	}
	event.Skip();
}

// Mouse button up event.
void
wxViewPortControl::OnMouseReleaseEvent(wxMouseEvent& event)
{
	event.Skip();
	if (!event.LeftUp())
	{
		return;
	}

	if (HasCapture())
	{
		ReleaseMouse();
	}
	m_isPlotAreaMouseDown = false;

	// Get the wxChartViewer zoom/scroll state to determine which type of mouse action is allowed
	SyncState();

	// Handle the mouse up event
	handleMouseUp(ToImageX(event.GetX()), ToImageY(event.GetY()));

	// Update the chart viewer if the viewport has changed
	UpdateChartViewerIfNecessary();
}

// MouseWheel handler
void
wxViewPortControl::OnMouseWheelEvent(wxMouseEvent& event)
{
	event.Skip();
	// Process the mouse wheel only if the mouse is over the plot area
	if (!m_chartViewer || (!isOnPlotArea(event.GetX(), event.GetY())))
	{
		// event.Skip();
	}
	else
	{
		// Ask the wxChartViewer to zoom around the center of the chart
		int x = m_chartViewer->getPlotAreaLeft() + m_chartViewer->getPlotAreaWidth() / 2;
		int y = m_chartViewer->getPlotAreaTop() + m_chartViewer->getPlotAreaHeight() / 2;
		if (!m_chartViewer->onMouseWheelZoom(x, y, event.GetWheelRotation()))
		{
			// event.Skip();
		}
	}
}

// Get the wxChartViewer zoom/scroll state
void
wxViewPortControl::SyncState()
{
	wxChartViewer* viewer = getViewer();
	if (NULL != viewer)
	{
		setZoomScrollDirection(viewer->getZoomDirection(), viewer->getScrollDirection());
	}
}

// Determines if the mouse is dragging.
bool
wxViewPortControl::IsDrag(wxMouseEvent& event)
{
	wxChartViewer* viewer = getViewer();
	if (NULL == viewer)
	{
		return false;
	}

	int minimumDrag = viewer->getMinimumDrag();
	int moveX = abs(m_mouseDownX - event.GetX());
	int moveY = abs(m_mouseDownY - event.GetY());
	return (moveX >= minimumDrag) || (moveY >= minimumDrag);
}

// Update the display
void
wxViewPortControl::updateDisplay()
{
	paintViewPort();
	PaintDisplay();
}

// Update the CChartViewer if the viewport has changed
void
wxViewPortControl::UpdateChartViewerIfNecessary()
{
	wxChartViewer* viewer = getViewer();
	if (!viewer)
	{
		return;
	}

	if (needUpdateChart() || needUpdateImageMap())
	{
		viewer->updateViewPort(needUpdateChart(), needUpdateImageMap());
	}
}

// Update the mouse cursor
void
wxViewPortControl::UpdateCursor(int position)
{
	switch (position)
	{
	case Chart::Left:
	case Chart::Right:
		SetCursor(wxCursor(wxCURSOR_SIZEWE));
		break;
	case Chart::Top:
	case Chart::Bottom:
		SetCursor(wxCursor(wxCURSOR_SIZENS));
		break;
	case Chart::TopLeft:
	case Chart::BottomRight:
		SetCursor(wxCursor(wxCURSOR_SIZENWSE));
		break;
	case Chart::TopRight:
	case Chart::BottomLeft:
		SetCursor(wxCursor(wxCURSOR_SIZENESW));
		break;
	default:
		SetCursor(wxCursor(wxCURSOR_ARROW));
		break;
	}
}
