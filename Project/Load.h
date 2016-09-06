#ifndef LOAD_H
#define LOAD_H

#include "Shunt.h"

class Load : public Shunt
{
   public:
    Load();
    ~Load();

    virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual void Rotate(bool clockwise = true);
    virtual bool GetContextMenu(wxMenu& menu);

   private:
    std::vector<wxPoint2DDouble> m_triangPts;
};

#endif  // LOAD_H
