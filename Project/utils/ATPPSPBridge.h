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
#ifdef _WIN32
#include <windows.h>
#else
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <cerrno>
#endif

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

	bool Connect(unsigned timeout, unsigned retryInterval);
	void Disconnect();

	bool WaitATP();
	bool ReleaseATP();

	//void SetProcessHandle(HANDLE hProcess) { m_hATPProcess = hProcess; }
	void SetProcessId(long pid) { m_atpPid = pid; }

	SharedData& GetSharedData() const;

	bool IsConnected() const;

private:

#ifdef _WIN32
	HANDLE m_hATPReady = nullptr;
	HANDLE m_hPSPReady = nullptr;
	HANDLE m_hMapFile = nullptr;
#else
	sem_t* m_hATPReady = SEM_FAILED;
	sem_t* m_hPSPReady = SEM_FAILED;
	int m_hMapFile = -1;
#endif

	long m_atpPid = 0;

	SharedData* m_data = nullptr;
};

#endif	
