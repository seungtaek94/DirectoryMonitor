#include "pch.h"
#include "DirectoryMonitorThread.h"

CDirectoryMonitorThread::CDirectoryMonitorThread()
	: m_stopped(TRUE)
{
	m_nAddedFileCnt = 0;
	m_nRemovedFileCnt = 0;
	m_bWatchSubtree = FALSE;
	m_buffer.resize(sizeof(FILE_NOTIFY_INFORMATION) * 1024);
}

CDirectoryMonitorThread::~CDirectoryMonitorThread()
{

}

void CDirectoryMonitorThread::StartDirMonitor(CString strPath)
{
	m_strDirPath = strPath;
	this->start();
}

void CDirectoryMonitorThread::start()
{
	if(m_stopped == TRUE)
	{
		m_stopped = FALSE;
		m_thread.start(*this);
	}
}

FILE_ACTION_INFO CDirectoryMonitorThread::Pop()
{
	FILE_ACTION_INFO fileActionInfo = m_queue.front();
	m_queue.pop();
	return fileActionInfo;
}

void CDirectoryMonitorThread::stop()
{
	if (m_stopped == FALSE)
	{
		TRACE(_T("Monitoring Thread Stopped !!"));

		m_stopped = TRUE;

		CloseHandle(m_hDirMonitor);
		m_hDirMonitor = INVALID_HANDLE_VALUE;

		m_thread.join();		
	}
}

bool CDirectoryMonitorThread::isRunning()
{
	return m_stopped == FALSE;
}

BOOL CDirectoryMonitorThread::OpenDirMonitor()
{
	m_hDirMonitor = CreateFileW(
		m_strDirPath,
		FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
		NULL
	);

	if (m_hDirMonitor == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	m_dwNotifyFilter =
		FILE_NOTIFY_CHANGE_CREATION |
		FILE_NOTIFY_CHANGE_LAST_WRITE |
		FILE_NOTIFY_CHANGE_SIZE |
		FILE_NOTIFY_CHANGE_DIR_NAME |
		FILE_NOTIFY_CHANGE_FILE_NAME;

	overlapped.hEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

	return TRUE;
}

void CALLBACK CDirectoryMonitorThread::DirMonitorCallback(DWORD errorCode, DWORD bytesTransferred, LPOVERLAPPED overlapped)
{
	/*if (errorCode == ERROR_SUCCESS)
	{
		FILE_NOTIFY_INFORMATION* pfni;

			do
			{
				pfni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(&buffer[0]);

				switch (pfni->Action)
				{
				case FILE_ACTION_ADDED:
				{
					m_nAddedFileCnt++;

					strFileAction = _T("FILE_ACTION_ADDED");

					StringCbCopyNW(bufFileName, sizeof(bufFileName), pfni->FileName, pfni->FileNameLength);
					strLog.Format(_T("%s: %s - %d"), strFileAction, (CString)bufFileName, m_nAddedFileCnt);
					TRACE(strLog);

					g_pMainDlg->RecogData(bufFileName);
					break;
				}
				case FILE_ACTION_REMOVED:
				{
					m_nRemovedFileCnt++;

					strFileAction = _T("FILE_ACTION_REMOVED");
					StringCbCopyNW(bufFileName, sizeof(bufFileName), pfni->FileName, pfni->FileNameLength);
					strLog.Format(_T("%s: %s - %d"), strFileAction, (CString)bufFileName, m_nRemovedFileCnt);
					TRACE(strLog);

					break;
				}
				default:
					break;
				}

				if (pfni->NextEntryOffset == 0)
					break;

				pfni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(reinterpret_cast<BYTE*>(pfni) + pfni->NextEntryOffset);

			} while (TRUE);
		}
	}*/
}

BOOL CDirectoryMonitorThread::ListenDirAltered()
{
	BOOL success = ReadDirectoryChangesW(m_hDirMonitor, &m_buffer[0], (DWORD)m_buffer.size(), m_bWatchSubtree, m_dwNotifyFilter, &bytesReturned, &overlapped, 0);

	if (success) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL CDirectoryMonitorThread::IsEmpty()
{
	return m_queue.empty();
}

void CDirectoryMonitorThread::run()
{
	if (OpenDirMonitor() == FALSE) {
		TRACE(L"모니터링 폴더가 잘못되었습니다. !");
		m_stopped = TRUE;
	}
	
	while (m_stopped == FALSE)
	{
		if (ListenDirAltered() == FALSE) {
			DWORD dwLastError = GetLastError();
			TRACE(_T("Dr Monitor Error !!"));
			break;
		}

		FILE_NOTIFY_INFORMATION* pfni;
		FILE_ACTION_INFO _fileActiontInfo;		

		if (WaitForSingleObject(overlapped.hEvent, INFINITE) != WAIT_OBJECT_0)
		{
			continue;		
		}

		if (!GetOverlappedResult(m_hDirMonitor, &overlapped, &bytesReturned, TRUE)) {
			continue;
		}

		if (bytesReturned == 0)
			break;

		pfni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(&m_buffer[0]);

		do
		{
			_fileActiontInfo = GetFileActionInfo(pfni);

			if (_fileActiontInfo.timeAction != NULL) {
				m_queue.push(_fileActiontInfo);
			}

			if (pfni->NextEntryOffset == 0)
				break;

			pfni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(reinterpret_cast<BYTE*>(pfni) + pfni->NextEntryOffset);

		} while (m_stopped == FALSE);
	}
}

FILE_ACTION_INFO CDirectoryMonitorThread::GetFileActionInfo(FILE_NOTIFY_INFORMATION* pfni)
{
	CString strFileAction;
	CString strLog;
	FILE_ACTION_INFO _fileActiontInfo;

	switch (pfni->Action)
	{
	case FILE_ACTION_ADDED:
	{
		m_nAddedFileCnt++;

		strFileAction = _T("FILE_ACTION_ADDED");

		CString _fileName(pfni->FileName, pfni->FileNameLength);
		CString _filePath(m_strDirPath + _T("\\") + _fileName);

		strLog.Format(_T("%s: %s - %d\n"), strFileAction.GetBuffer(0), _fileName.GetBuffer(0), m_nAddedFileCnt);
		TRACE(strLog);

		_fileActiontInfo = { m_timeCurret.GetCurrentTime(), FILE_ACTION_ADDED, _fileName, _filePath };

		return _fileActiontInfo;
	}
	case FILE_ACTION_REMOVED:
	{
		m_nRemovedFileCnt++;
		strFileAction = _T("FILE_ACTION_REMOVED");

		CString _fileName(pfni->FileName, pfni->FileNameLength);
		CString _filePath(m_strDirPath + _T("\\") + _fileName);

		strLog.Format(_T("%s: %s - %d\n"), strFileAction.GetBuffer(0), _fileName.GetBuffer(0), m_nRemovedFileCnt);
		TRACE(strLog);

		_fileActiontInfo = { m_timeCurret.GetCurrentTime(), FILE_ACTION_ADDED, _fileName, _filePath };
		return _fileActiontInfo;
	}
	default:
		break;
	}

	return _fileActiontInfo;
}