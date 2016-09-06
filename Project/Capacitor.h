#ifndef CAPACITOR_H
#define CAPACITOR_H

#include "Shunt.h"

class Capacitor : public Shunt
{
public:
	Capacitor();
	~Capacitor();
	
	virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const;
	virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void Rotate(bool clockwise = true);
    virtual bool GetContextMenu(wxMenu& menu);

};

#endif // CAPACITOR_H
