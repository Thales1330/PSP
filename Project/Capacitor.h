#ifndef CAPACITOR_H
#define CAPACITOR_H

#include "Shunt.h"

class Capacitor : Shunt
{
public:
	Capacitor();
	~Capacitor();
	
	virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual void Rotate();
    virtual bool GetContextMenu(wxMenu& menu);

};

#endif // CAPACITOR_H
