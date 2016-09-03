#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include "Branch.h"

class Transformer : public Branch
{
public:
   Transformer();
    virtual ~Transformer();
	
    virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void Rotate();
	virtual void Move(wxPoint2DDouble position);
	virtual void MoveNode(Element* parent, wxPoint2DDouble position);
	virtual void StartMove(wxPoint2DDouble position);
	virtual bool GetContextMenu(wxMenu& menu);
};

#endif  // TRANSFORMER_H
