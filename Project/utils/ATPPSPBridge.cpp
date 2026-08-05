#include "ATPPSPBridge.h"
#include <wx/process.h>


namespace
{
#ifdef _WIN32

	constexpr wchar_t ATP_EVENT[] = L"PSPBridge_ATPReady";
	constexpr wchar_t PSP_EVENT[] = L"PSPBridge_PSPReady";
	constexpr wchar_t SHM_NAME[] = L"PSPBridgeSharedMemory";

#else

	// POSIX object names must begin with '/'
	constexpr char ATP_EVENT[] = "/PSPBridge_ATPReady";
	constexpr char PSP_EVENT[] = "/PSPBridge_PSPReady";
	constexpr char SHM_NAME[] = "/PSPBridgeSharedMemory";

#endif
}

ATPPSPBridge::ATPPSPBridge()
{
}

ATPPSPBridge::~ATPPSPBridge()
{
	Disconnect();
}

bool ATPPSPBridge::Connect(unsigned timeout, unsigned retryInterval)
{
	wxStopWatch sw;
	wxProgressDialog wait(_("Connecting"), _("Connecting to ATP, please wait."), timeout, nullptr, wxPD_AUTO_HIDE | wxPD_SMOOTH | wxPD_CAN_ABORT);

	while (true)
	{
		if (!wait.Update(sw.Time()))
			return false;

		Disconnect();

#ifdef _WIN32

		// ATP event
		m_hATPReady = OpenEventW(SYNCHRONIZE, FALSE, ATP_EVENT);

		if (m_hATPReady)
		{
			// PSP event
			m_hPSPReady = OpenEventW(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, PSP_EVENT);

			if (m_hPSPReady)
			{
				// Shared memory
				m_hMapFile = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, SHM_NAME);

				if (m_hMapFile)
				{
					m_data = static_cast<SharedData*>(MapViewOfFile(m_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0));

					if (m_data)
						return true;
				}
			}
		}

#else

		// ATP semaphore
		m_hATPReady = sem_open(ATP_EVENT, 0);

		if (m_hATPReady != SEM_FAILED)
		{
			// PSP semaphore
			m_hPSPReady = sem_open(PSP_EVENT, 0);

			if (m_hPSPReady != SEM_FAILED)
			{
				// Shared memory
				m_hMapFile = shm_open(SHM_NAME, O_RDWR, 0);
				
				if (m_hMapFile >= 0)
				{
					m_data = nullptr;
					struct stat st;
					if (fstat(m_hMapFile, &st) == 0)
						m_data = static_cast<SharedData*>(mmap(nullptr, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_hMapFile, 0));

					if (m_data != MAP_FAILED)
						return true;

					m_data = nullptr;
				}
			}
		}

#endif

		Disconnect();

		if (sw.Time() >= timeout)
			return false;

		wxMilliSleep(retryInterval);
		wxTheApp->Yield();
	}
}

void ATPPSPBridge::Disconnect()
{
#ifdef _WIN32

	if (m_data) {
		UnmapViewOfFile(m_data);
		m_data = nullptr;
	}

	if (m_hMapFile) {
		CloseHandle(m_hMapFile);
		m_hMapFile = nullptr;
	}

	if (m_hATPReady) {
		CloseHandle(m_hATPReady);
		m_hATPReady = nullptr;
	}

	if (m_hPSPReady) {
		CloseHandle(m_hPSPReady);
		m_hPSPReady = nullptr;
	}

#else

	if (m_data) {
		struct stat st;
		if (m_hMapFile >= 0 && fstat(m_hMapFile, &st) == 0)
			munmap(m_data, st.st_size);

		m_data = nullptr;
	}

	if (m_hMapFile >= 0) {
		close(m_hMapFile);
		m_hMapFile = -1;
	}

	if (m_hATPReady != SEM_FAILED) {
		sem_close(m_hATPReady);
		m_hATPReady = SEM_FAILED;
	}

	if (m_hPSPReady != SEM_FAILED) {
		sem_close(m_hPSPReady);
		m_hPSPReady = SEM_FAILED;
	}

#endif
	m_atpPid = 0;
}

bool ATPPSPBridge::WaitATP()
{
#ifdef _WIN32

	if (!m_hATPReady)
		return false;

	while (true)
	{
		DWORD ret = WaitForSingleObject(m_hATPReady, 100);

		switch (ret)
		{
		case WAIT_OBJECT_0:
			// ATP signaled
			return true;

		case WAIT_TIMEOUT:
			// Check whether ATP is still running
			if (m_atpPid != 0 && !wxProcess::Exists(m_atpPid))
				return false;
			break;

		default:
			return false;
		}
	}

#else

	if (m_hATPReady == SEM_FAILED)
		return false;

	while (true)
	{
		timespec ts;
		if (clock_gettime(CLOCK_REALTIME, &ts) != 0)
			return false;

		ts.tv_nsec += 100000000; // 100 ms

		if (ts.tv_nsec >= 1000000000)
		{
			ts.tv_sec++;
			ts.tv_nsec -= 1000000000;
		}

		if (sem_timedwait(m_hATPReady, &ts) == 0)
			return true;

		if (errno != ETIMEDOUT)
			return false;

		// Check whether ATP is still running
		if (m_atpPid != 0 && !wxProcess::Exists(m_atpPid))
			return false;
	}

#endif
}

bool ATPPSPBridge::ReleaseATP()
{
#ifdef _WIN32

	if (!m_hPSPReady)
		return false;

	return SetEvent(m_hPSPReady) != FALSE;

#else

	if (m_hPSPReady == SEM_FAILED)
		return false;

	return sem_post(m_hPSPReady) == 0;

#endif
}

SharedData& ATPPSPBridge::GetSharedData() const
{
	return *m_data;
}

bool ATPPSPBridge::IsConnected() const
{
	return m_data != nullptr;
}