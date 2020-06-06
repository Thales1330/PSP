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

#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include "Branch.h"

class TransformerForm;

enum TransformerConnection {
    GWYE_GWYE = 0,
    WYE_GWYE,
    GWYE_WYE,
    WYE_WYE,
    DELTA_GWYE,
    DELTA_WYE,
    GWYE_DELTA,
    WYE_DELTA,
    DELTA_DELTA
};

struct TransformerElectricalData {
    // General
    wxString name = "";
    double primaryNominalVoltage = 138.0;
    ElectricalUnit primaryNominalVoltageUnit = ElectricalUnit::UNIT_kV;
    double secondaryNominalVoltage = 138.0;
    ElectricalUnit secondaryNominalVoltageUnit = ElectricalUnit::UNIT_kV;
    int baseVoltage = 0;
    double nominalPower = 100.0;
    ElectricalUnit nominalPowerUnit = ElectricalUnit::UNIT_MVA;
    double resistance = 0.0;
    ElectricalUnit resistanceUnit = ElectricalUnit::UNIT_PU;
    double indReactance = 1.0;
    ElectricalUnit indReactanceUnit = ElectricalUnit::UNIT_PU;
    TransformerConnection connection = GWYE_GWYE;
    double turnsRatio = 1.0;
    double phaseShift = 0.0;
    bool useTransformerPower = false;

    // Power flow (p.u.)
    std::complex<double> current[2] = {std::complex<double>(0.0, 0.0), std::complex<double>(0.0, 0.0)};
    std::complex<double> powerFlow[2] = {std::complex<double>(0.0, 0.0), std::complex<double>(0.0, 0.0)};

    // Fault
    double zeroResistance = 0.0;
    double zeroIndReactance = 1.0;
    double primaryGrndResistance = 0.0;
    double primaryGrndReactance = 0.0;
    double secondaryGrndResistance = 0.0;
    double secondaryGrndReactance = 0.0;
    // p.u. fault data
    std::complex<double> faultCurrent[2][3];
};

/**
 * @class Transformer
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 06/10/2017
 * @brief Two-winding transformer power element.
 * @file Transformer.h
 */
class Transformer : public Branch
{
   public:
    Transformer();
    Transformer(wxString name);
    virtual ~Transformer();

    virtual Element* GetCopy();
    virtual bool AddParent(Element* parent, wxPoint2DDouble position);
    virtual bool Contains(wxPoint2DDouble position) const;
    virtual void Draw(wxPoint2DDouble translation, double scale) const;
    virtual void DrawDC(wxPoint2DDouble translation, double scale, wxGraphicsContext* gc) const;
    virtual bool Intersects(wxRect2DDouble rect) const;
    virtual void Rotate(bool clockwise = true);
    virtual void Move(wxPoint2DDouble position);
    virtual void MoveNode(Element* parent, wxPoint2DDouble position);
    virtual void StartMove(wxPoint2DDouble position);
    virtual bool GetContextMenu(wxMenu& menu);
    virtual wxString GetTipText() const;
    virtual void RotateNode(Element* parent, bool clockwise);
    virtual bool SetNodeParent(Element* parent);
    virtual void SetPowerFlowDirection(PowerFlowDirection pfDirection);
    virtual bool ShowForm(wxWindow* parent, Element* element);
    virtual TransformerElectricalData GetElectricalData() const { return m_electricalData; }
    virtual TransformerElectricalData GetPUElectricalData(double systemBasePower);
    virtual void SetElectricaData(TransformerElectricalData electricalData) { m_electricalData = electricalData; }
    virtual void SetNominalVoltage(std::vector<double> nominalVoltage, std::vector<ElectricalUnit> nominalVoltageUnit);
    virtual void SetBestPositionAndRotation();

    virtual rapidxml::xml_node<>* SaveElement(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* elementListNode);
    virtual bool OpenElement(rapidxml::xml_node<>* elementNode, std::vector<Element*> parentList);

   protected:
    void UpdatePowerFlowArrowsPosition();
    TransformerElectricalData m_electricalData;
};

#endif  // TRANSFORMER_H
