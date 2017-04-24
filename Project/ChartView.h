#ifndef CHARTVIEW_H
#define CHARTVIEW_H
#include "ChartViewBase.h"

class ChartView : public ChartViewBase
{
public:
    ChartView(wxWindow* parent);
    virtual ~ChartView();
};
#endif // CHARTVIEW_H
