#include "ATPPSPBridge.h"


namespace
{
	constexpr wchar_t ATP_EVENT[] = L"PSPBridge_ATPReady";
	constexpr wchar_t PSP_EVENT[] = L"PSPBridge_PSPReady";
	constexpr wchar_t SHM_NAME[] = L"PSPBridgeSharedMemory";
}

ATPPSPBridge::ATPPSPBridge()
{
}

ATPPSPBridge::~ATPPSPBridge()
{
	Disconnect();
}

bool ATPPSPBridge::Connect(DWORD timeout, DWORD retryInterval)
{
	wxStopWatch sw;
	//wxBusyInfo wait(_("Connecting to ATP..."));
	wxProgressDialog wait(_("Connecting"), _("Connecting to ATP, please wait."), timeout, nullptr,  wxPD_AUTO_HIDE | wxPD_SMOOTH | wxPD_CAN_ABORT);

	while (true)
	{
		if (!wait.Update(sw.Time()))
			return false;
		Disconnect();

		// ATP event
		m_hATPReady = OpenEventW(SYNCHRONIZE, FALSE, ATP_EVENT);

		if (m_hATPReady)
		{
			// PSP event
			m_hPSPReady = OpenEventW(
				EVENT_MODIFY_STATE | SYNCHRONIZE,
				FALSE,
				PSP_EVENT);

			if (m_hPSPReady)
			{
				// Shared memory
				m_hMapFile = OpenFileMappingW(
					FILE_MAP_ALL_ACCESS,
					FALSE,
					SHM_NAME);

				if (m_hMapFile)
				{
					m_data = static_cast<SharedData*>(
						MapViewOfFile(
							m_hMapFile,
							FILE_MAP_ALL_ACCESS,
							0,
							0,
							0));

					if (m_data) {
						return true;
					}
				}
			}
		}

		Disconnect();

		if (sw.Time() >= timeout) {
			return false;
		}

		wxMilliSleep(retryInterval);
		wxTheApp->Yield();
	}

}

void ATPPSPBridge::Disconnect()
{
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
}

bool ATPPSPBridge::WaitATP()
{
	//if (!m_hATPReady)
	//	return false;
	//
	//return WaitForSingleObject(m_hATPReady, timeout) == WAIT_OBJECT_0;
	if (!m_hATPReady)
		return false;

	if (!m_hATPProcess)
	{
		return WaitForSingleObject(m_hATPReady, INFINITE) == WAIT_OBJECT_0;
	}

	HANDLE handles[2] =
	{
		m_hATPReady,
		m_hATPProcess
	};

	DWORD ret = WaitForMultipleObjects(
		2,
		handles,
		FALSE,
		INFINITE);

	switch (ret)
	{
	case WAIT_OBJECT_0:
		// ATP envent
		return true;

	case WAIT_OBJECT_0 + 1:
		// Process finished
		return false;

	default:
		return false;
	}
}

bool ATPPSPBridge::ReleaseATP()
{
	if (!m_hPSPReady)
		return false;

	return SetEvent(m_hPSPReady) != FALSE;
}

SharedData& ATPPSPBridge::GetSharedData() const
{
	return *m_data;
}

bool ATPPSPBridge::IsConnected() const
{
	return m_data != nullptr;
}