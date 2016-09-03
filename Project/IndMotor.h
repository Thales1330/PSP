#ifndef INDMOTOR_H
#define INDMOTOR_H

#include "Machines.h"

class IndMotor : public Machines
{
public:
	IndMotor();
	~IndMotor();
	
	virtual void DrawSymbol() const;
	virtual bool GetContextMenu(wxMenu& menu);
};

#endif // INDMOTOR_H
