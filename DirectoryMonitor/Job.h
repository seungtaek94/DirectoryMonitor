#if !defined(__JOB_H__)
#define __JOB_H__

#include "poco/Runnable.h"
#include "poco/RefCountedObject.h"
#include "poco/AutoPtr.h"


namespace POCO
{
	class Job
		: public Poco::Runnable
		, public Poco::RefCountedObject
	{
	public:
		typedef Poco::AutoPtr<Job> PTR;

		Job();
		void run();

	protected:
		virtual ~Job();
	};
}
#endif