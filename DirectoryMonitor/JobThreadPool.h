#if !defined(__JOB_THREAD_POOL_H__)
#define __JOB_THREAD_POOL_H__

#include "poco/NotificationQueue.h"
#include "poco/ThreadPool.h"
#include "poco/Mutex.h"
#include "poco/Timestamp.h"

#include "Job.h"

namespace POCO
{
	class JobNotification
		: public Poco::Notification
	{
	public:
		JobNotification(Job::PTR& job);
		Job::PTR& job();

	protected:
		~JobNotification();

	protected:
		Job::PTR m_job;
	};

	class JobThreadPool
		: public Poco::ThreadPool
		, public Poco::Runnable
	{
	public:
		JobThreadPool(Int32 min = 1, Int32 max = 16);
		JobThreadPool(const std::string& name, Int32 min = 1, Int32 max = 16);
		~JobThreadPool();

		virtual void start();
		virtual void stop();
		virtual bool isRunning();

		void push(Job::PTR job);
	protected:
		void run();

	private:
		void start(Poco::Runnable& target);
		void start(Poco::Runnable& target, const std::string& name);
		void startWithPriority(Poco::Thread::Priority priority, Poco::Runnable& target);
		void startWithPriority(Poco::Thread::Priority priority, Poco::Runnable& target, const std::string name);

	protected:
		bool m_stopped;

		Poco::NotificationQueue m_queue;
		Poco::FastMutex m_mutex;
	};
}
#endif