#ifndef SYNCGENERATOR_H
#define SYNCGENERATOR_H

#include "Machines.h"

class SyncGenerator : public Machines
{
   public:
    SyncGenerator();
    ~SyncGenerator();
	virtual void DrawSymbol() const;
	virtual bool GetContextMenu(wxMenu& menu);

   private:
    std::vector<wxPoint2DDouble> m_sinePts;
};

#endif  // SYNCGENERATOR_H
