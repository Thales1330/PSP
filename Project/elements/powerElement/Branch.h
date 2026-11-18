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

// Fuse family (Annex H). Order MUST match arcflash::FuseFamily exactly
enum class ArcFlashFuseFamily {
	CLASS_L_601_800A,
	CLASS_L_801_1200A,
	CLASS_L_1201_1600A,
	CLASS_L_1601_2000A,
	CLASS_RK1_UPTO100A,
	CLASS_RK1_101_200A,
	CLASS_RK1_201_400A,
	CLASS_RK1_401_600A
};

// Breaker family (Annex I). Order MUST match arcflash::BreakerFamily exactly.
enum class ArcFlashBreakerFamily {
	MCCB_100_400A,
	MCCB_600_1200A_TM_OR_M,
	MCCB_600_1200A_E_LI,
	MCCB_OR_ICCB_1600_6000A,
	LVPCB_800_6300A_E_LI,
	LVPCB_800_6300A_E_LS
};

// Order MUST match arcflash::BreakerVoltageBand exactly.
enum class ArcFlashBreakerVoltageBand {
	V480_AND_LOWER,
	V575_TO_690
};

struct ArcFlashProtectionData {
	bool consider = false;    // whether this path's protection config participates 
	bool configured = false;

	ArcFlashProtectionType type = ArcFlashProtectionType::CIRCUIT_BREAKER;
	ArcFlashProtectionMethod method = ArcFlashProtectionMethod::TCC_CURVE;

	// --- TCC_CURVE method ---
	std::vector<ArcFlashTCCPoint> tccPoints;
	bool curveIsMeltTimeOnly = false;   // fuses only (6.9.2 melt->clearing adjustment)
	double additionalDelay = 0.0;       // s; retrofit breaker opening time (6.9.3) or
	// relay+breaker interrupting time (6.9.4)

// Reserved for future UI exposure
	double floorSeconds = 0.01;
	double practicalCapSeconds = -1.0;  // <=0 disables the cap
	int belowRangePolicy = 0;           // 0 = ThrowError, 1 = ClampToNearest
	int aboveRangePolicy = 1;           // 0 = ThrowError, 1 = ClampToNearest

	// --- ANNEX_H method (fuse) ---
	ArcFlashFuseFamily fuseFamily = ArcFlashFuseFamily::CLASS_L_601_800A;

	// --- ANNEX_I method (breaker) ---
	ArcFlashBreakerFamily breakerFamily = ArcFlashBreakerFamily::MCCB_100_400A;
	ArcFlashBreakerVoltageBand breakerVoltageBand = ArcFlashBreakerVoltageBand::V480_AND_LOWER;
	double i2_kA = 0.0;                 // interrupting rating — was missing, required by both
	// CalcSpecialBreakerModel overloads
	ArcFlashI1Source i1Source = ArcFlashI1Source::DIRECT;
	double i1_kA = 0.0;
	double it_A = 0.0;
	double breakerNominalCurrent_A = 0.0; // A; optional, used to determine the default It

	// --- Current distribution (6.8/6.10) ---
	double faultCurrentContribution_kA = 0.0;
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
