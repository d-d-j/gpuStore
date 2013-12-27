#ifndef STORETASK_H_
#define STORETASK_H_

#include "TaskResult.h"
#include <boost/thread.hpp>
#include <boost/utility.hpp>
#include <cstring>

namespace ddj {
namespace task {

	class Task : public boost::noncopyable
	{
	private:
		/* TASK */
		int64_t _taskId;
		TaskType _type;
		void* _taskData;

		/* RESULT */
		void* _resultData;
		size_t _resultSize;
		char* _message;
		bool _isSuccessfull;
		bool _isCompleated;
		int _currentResultCount;
		int _expectedResultCount;

		/* MONITOR */
		boost::condition_variable* _condResponseReady;
		boost::mutex _mutex;

	public:
		Task(
				int taskId,
				TaskType type,
				void* taskData,
				int expectedResultCount,
				boost::condition_variable* cond);

		virtual ~Task();

		void SetResult(
				bool isSuccessfull,
				const char* message = nullptr,
				void* resultData = nullptr,
				size_t resultSize = 0);

		taskResult* GetResult();

		/* GETTERS */
		TaskType GetType();
		void* GetData();
		int GetDevice();
		bool IsCompleated();

	private:
		void appendMessage(char* message);
	};

	typedef boost::shared_ptr<Task> Task_Pointer;

} /* namespace store */
} /* namespace ddj */

#endif /* STORETASK_H_ */
