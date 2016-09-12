#ifndef INDUCTOR_H
#define INDUCTOR_H

#include "Shunt.h"

class Inductor : public Shunt
{
public:
	Inductor();
	~Inductor();
	
	virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
	virtual bool Contains(wxPoint2DDouble position) const;
	virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void Rotate(bool clockwise = true);
    virtual bool GetContextMenu(wxMenu& menu);
};

#endif // INDUCTOR_H