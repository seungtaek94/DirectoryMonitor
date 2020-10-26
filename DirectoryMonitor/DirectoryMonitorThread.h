#pragma once

#include "pch.h"
#include "poco/Thread.h"
#include "poco/Runnable.h"
#include "poco/Mutex.h"

#include <queue>

using namespace std;

class CDirectoryMonitorThread : public Poco::Runnable
{
public:
	CDirectoryMonitorThread();
	~CDirectoryMonitorThread();

public:
	void StartDirMonitor(CString strPath);
	void stop();
	bool isRunning();

	BOOL IsEmpty();
	FILE_ACTION_INFO Pop();	

	int GetAddedFileNum() { return m_nAddedFileCnt; };
	int GetRemovedFileNum() { return m_nRemovedFileCnt; };

private:
	DWORD bytesReturned = 0;
	OVERLAPPED overlapped{ 0 };

	void start();
	void run();	
	BOOL OpenDirMonitor();
	BOOL ListenDirAltered();

	FILE_ACTION_INFO GetFileActionInfo(FILE_NOTIFY_INFORMATION* pfni);
	static void CALLBACK DirMonitorCallback(DWORD, DWORD, LPOVERLAPPED);

	HANDLE m_hDirMonitor;
	BOOL m_bWatchSubtree;
	DWORD m_dwNotifyFilter;

	CTime m_timeCurret;

	int m_nAddedFileCnt;
	int m_nRemovedFileCnt;

	std::vector<BYTE> m_buffer;
	Poco::Thread m_thread;
	Poco::FastMutex m_mutexDirMonitor;

	CWinThread* m_pListenDirAlterdThread;

	typedef std::queue<FILE_ACTION_INFO> QUEUE;
	QUEUE m_queue;

protected:
	CString m_strDirPath;	
	BOOL m_bOngoingFileAdd;
	BOOL m_stopped;
};

