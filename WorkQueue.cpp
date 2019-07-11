#include "WorkQueue.h"
#include "WorkQueueClass.h"

namespace Utility {

	static Utility::WorkQueue MyQueue;
	typedef std::function<void()> MyFunction;

	void WorkQueueEnqueue(MyFunction &work) {
		MyQueue.Enqueue(work);
	}

	void WorkQueueEnqueue(MyFunction &&work) {
		MyQueue.Enqueue(std::move(work));
	}

	void WorkQueueDequeue() {
		MyQueue.Dequeue();
	}

	void WorkQueueRunWork() {
		MyQueue.RunWork();
	}

	void WorkQueuePop() {
		MyQueue.Pop();
	}

}
