#ifndef SYNCMOTOR_H
#define SYNCMOTOR_H

#include "Machines.h"

class SyncMotor : public Machines
{
public:
	SyncMotor();
	~SyncMotor();
	
	virtual void DrawSymbol() const;
	virtual bool GetContextMenu(wxMenu& menu);

};

#endif // SYNCMOTOR_H
