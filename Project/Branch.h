/*
 *  Copyright (C) 2017  Thales Lima Oliveira <thales@ufu.br>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BRANCH_H
#define BRANCH_H

#include "PowerElement.h"
#include "Bus.h"

class Branch : public PowerElement
{
   public:
    Branch();
    ~Branch();

    virtual bool Contains(wxPoint2DDouble position) const { return false; }
    virtual void Draw(wxPoint2DDouble translation, double scale) const {}
    virtual void Move(wxPoint2DDouble position) {}
    virtual void StartMove(wxPoint2DDouble position) {}
    virtual void MoveNode(Element* parent, wxPoint2DDouble position) {}
    virtual bool NodeContains(wxPoint2DDouble position);
    virtual bool SetNodeParent(Element* parent);
    virtual void RemoveParent(Element* parent);
    virtual void UpdateNodes();
    virtual wxCursor GetBestPickboxCursor() const { return wxCURSOR_ARROW; }
    virtual bool Intersects(wxRect2DDouble rect) const { return false; }
    virtual void MovePickbox(wxPoint2DDouble position) {}
    virtual bool PickboxContains(wxPoint2DDouble position) { return false; }
    virtual void RotateNode(Element* parent, bool clockwise = true);
    virtual void AddPoint(wxPoint2DDouble point){};
    virtual bool GetContextMenu(wxMenu& menu) { return false; }
    virtual void UpdateSwitchesPosition();
    virtual void UpdateSwitches();

   protected:
    bool m_inserted = false;
};

#endif  // BRANCH_H
