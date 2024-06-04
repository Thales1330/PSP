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

#ifndef MACHINES_H
#define MACHINES_H

#include "PowerElement.h"

class ControlElementContainer;
class ControlElementSolver;

/**
 * @class Machines
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 06/10/2017
 * @brief Abstract class for rotary machines power elements.
 * @file Machines.h
 */
class Machines : public PowerElement
{
   public:
    enum SyncMachineModel { SM_MODEL_1 = 0, SM_MODEL_2, SM_MODEL_3, SM_MODEL_4, SM_MODEL_5 };

    Machines();
    virtual ~Machines() {}

    virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual bool Contains(wxPoint2DDouble position) const { return m_rect.Contains(position); }
    //virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual void DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext*  gc) const;
    virtual bool Intersects(wxRect2DDouble rect) const { return m_rect.Intersects(rect); }
    virtual void Move(wxPoint2DDouble position);
    virtual void MoveNode(Element* element, wxPoint2DDouble position);
    virtual void StartMove(wxPoint2DDouble position);
    virtual void RotateNode(Element* parent, bool clockwise = true);
    virtual void RemoveParent(Element* parent);
    virtual bool NodeContains(wxPoint2DDouble position);
    virtual bool SetNodeParent(Element* parent);
    virtual void UpdateNodes();
    virtual void Rotate(bool clockwise = true);
    //virtual void DrawSymbol() const {}
    virtual void DrawDCSymbol(wxGraphicsContext* gc) const {}
    virtual void SetPowerFlowDirection(PowerFlowDirection pfDirection);

   protected:
    void UpdateSwitchesPosition();
    void UpdatePowerFlowArrowsPosition();
    bool m_inserted = false;
};

#endif  // MACHINES_H
