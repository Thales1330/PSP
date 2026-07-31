/*
 *  Copyright (C) 2026  Thales Lima Oliveira <thales@ufu.br>
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

#ifndef ATPPSPBRIDGE_H
#define ATPPSPBRIDGE_H

#include <cstdint>
#include <wx/app.h>
#include <wx/stopwatch.h>
#include <wx/progdlg.h>
#include <windows.h>

enum class Mode
{
    RawCurrent = 0,
    Phasor = 1,
    Both = 2
};

struct Sample
{
    double current[3];
};

struct Phasor
{
    double Id[3];
    double Iq[3];
};

struct SharedData
{
	std::uint32_t nPh;

    double t;

    double vrms;
    double freq;
    double theta;
    double phase;

    double stoptime;
    double atpStepsize;
    double pspStepsize;

    Mode mode;
    int terminate;

    Phasor phasor;

    std::uint32_t stepCount;
    std::uint32_t syncSteps;

    Sample samples[];
};

class ATPPSPBridge
{
public:

	ATPPSPBridge();
	~ATPPSPBridge();

	bool Connect(DWORD timeout, DWORD retryInterval);
	void Disconnect();

	bool WaitATP();
	bool ReleaseATP();

    void SetProcessHandle(HANDLE hProcess) { m_hATPProcess = hProcess; }

	SharedData& GetSharedData() const;

	bool IsConnected() const;

private:

	HANDLE m_hATPReady = nullptr;
	HANDLE m_hPSPReady = nullptr;
	HANDLE m_hMapFile = nullptr;
    HANDLE m_hATPProcess = nullptr;

	SharedData* m_data = nullptr;
};

#endif	
