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


struct ArcFlashTCCPoint {
	double current = 0.0; // A
	double time = 0.0;    // s
};

enum class ArcFlashProtectionType {
	CIRCUIT_BREAKER,
	FUSE
};

enum class ArcFlashProtectionMethod {
	TCC_CURVE,
	ANNEX_H,
	ANNEX_I
};

enum class ArcFlashI1Source {
	DIRECT,
	FROM_IT
};

struct ArcFlashProtectionData {
	bool consider = false;
	bool configured = false;

	ArcFlashProtectionType type = ArcFlashProtectionType::CIRCUIT_BREAKER;
	ArcFlashProtectionMethod method = ArcFlashProtectionMethod::TCC_CURVE;

	std::vector<ArcFlashTCCPoint> tccPoints;

	bool curveIsMeltTimeOnly = false;
	double additionalDelay = 0.0;

	int fuseFamily = 0;
	int fuseCurrentRange = 0;

	int breakerFamily = 0;
	int breakerVoltageRange = 0;

	ArcFlashI1Source i1Source = ArcFlashI1Source::DIRECT;
	double i1_kA = 0.0;
	double it_A = 0.0;
};

/**
 * @class Branch
 * @author Thales Lima Oliveira <thales@ufu.br>
 * @date 06/10/2017
 * @brief Abstract class for branch power elements.
 * @file Branch.h
 */
class Branch : public PowerElement
{
public:
	Branch();
	~Branch();

	virtual bool Contains(wxPoint2DDouble position) const { return false; }
	//virtual void Draw(wxPoint2DDouble translation, double scale) const {}
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
	virtual void AddPoint(wxPoint2DDouble point) {};
	virtual bool GetContextMenu(wxMenu& menu) { return false; }
	virtual void UpdateSwitchesPosition();
	virtual void UpdateSwitches();
	//ArcFlashProtectionData& GetArcFlashProtectionData() { return arcFlashProtection; }

//protected:
	//ArcFlashProtectionData arcFlashProtection;
};

#endif  // BRANCH_H
