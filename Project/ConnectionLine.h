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

#ifndef CONNECTIONLINE_H
#define CONNECTIONLINE_H

#include "ControlElement.h"

/**
 * @class ConnectionLine
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 05/10/2017
 * @brief Connection between two control elements or other connection line and an element.
 * @file ConnectionLine.h
 */
class ConnectionLine : public ControlElement
{
   public:
    enum ConnectionLineType { ELEMENT_ELEMENT = 0, ELEMENT_LINE };
    ConnectionLine();
    ConnectionLine(Node* firstNode, int id);
    ~ConnectionLine();

    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void RemoveParent(Element* parent);
    virtual void StartMove(wxPoint2DDouble position);
    virtual void Move(wxPoint2DDouble position);
    virtual bool AppendNode(Node* node, ControlElement* parent);
    virtual void UpdatePoints();
    virtual void SetTemporarySecondPoint(wxPoint2DDouble point) { m_tmpSndPt = point; }
    virtual wxPoint2DDouble GetMidPoint() const;
    virtual double GetOffset() const { return m_lineOffset; }
    virtual void SetOffset(double offset) { m_lineOffset = offset; }
    virtual ConnectionLineType GetType() const { return m_type; }
    virtual void SetType(ConnectionLineType newType) { m_type = newType; }
    virtual ConnectionLine* GetParentLine() const { return m_parentLine; }
    virtual bool SetParentLine(ConnectionLine* parent);

    virtual std::vector<ConnectionLine*> GetLineChildList() const;

    virtual double GetValue() const { return m_value; }
    virtual void SetValue(double value) { m_value = value; }
    Element* GetCopy();

   protected:
    double m_lineOffset = 0.0;
    double m_moveStartPtY = 0.0;
    double m_moveStartOffset = 0.0;
    wxPoint2DDouble m_tmpSndPt;

    ConnectionLineType m_type = ELEMENT_ELEMENT;
    ConnectionLine* m_parentLine = NULL;

    double m_value;
};

#endif  // CONNECTIONLINE_H
