#include "pch.h"
#include "JobThreadPool.h"

#include "poco/Notification.h"

namespace POCO
{
	//JobNitification
	JobNotification::JobNotification(Job::PTR& job)
		: Poco::Notification()
		, m_job(job)
	{
	}

	Job::PTR& JobNotification::job()
	{
		return m_job;
	}

	JobNotification::~JobNotification()
	{
	}

	//JobThreadPool
	JobThreadPool::JobThreadPool(Int32 min /* = 1 */, Int32 max /* = 16 */)
		: Poco::ThreadPool(min, max)
		, Poco::Runnable()
		, m_stopped(true)
	{

	}

	JobThreadPool::JobThreadPool(const std::string& name, Int32 min /* = 1 */, Int32 max /* = 16*/)
		: Poco::ThreadPool(name, min, max)
		, Poco::Runnable()
		, m_stopped(true)
	{

	}

	JobThreadPool::~JobThreadPool()
	{
		if (m_stopped == false)
			stop();
	}

	void JobThreadPool::start()
	{
		m_stopped = false;
	}

	void JobThreadPool::stop()
	{
		//trace_out("stop try...");
		if (m_stopped == false)
		{
			m_stopped = true;
			m_queue.wakeUpAll();
			//trace_out("m_queue.wakeUpAll()");
			joinAll();
			//trace_out("joinAll()");
			m_queue.clear();
			//trace_out("m_queue.clear()");
		}
	}

	bool JobThreadPool::isRunning()
	{
		return m_stopped == false;
	}

	void JobThreadPool::push(Job::PTR job)
	{
		Poco::FastMutex::ScopedLock lock(m_mutex);
		if (isRunning() == false)
			return;

		if (m_queue.hasIdleThreads() == false && available() > 0)
			ThreadPool::start(*this);

		m_queue.enqueueNotification(new JobNotification(job));
	}

	void JobThreadPool::run()
	{
		while (m_stopped == false)
		{
			Poco::AutoPtr<JobNotification> notification = (JobNotification*)m_queue.waitDequeueNotification(1000);
			if (notification)
				notification->job()->run();
		}
	}
}