#pragma once

/** @file WorkQueueClass.h 
* WorkQueueClass.h --------------------------
* �֐������X�g�Ǘ����A���s�ł���悤�ɂ��܂��B
* ���O���:
*		Utility
* �g�p��:
* @code
*	#include "WorkQueueClass.h"
*
*	using namespace Utility;
*
*	void SomeFunction() {
*		printf("SomeFunction is called!\n");
*	}
*
*	class MyTask {
*		int Flames;
*	public:
*		MyTask() : Flames(0) {
*
*		}
*
*		bool operator()() {
*			Flames++;
*			printf("MyTask : %d\n", Flames);
*			return Flames >= 10;
*		}
*	};
*
*	int main() {
*		WorkQueue MyWorkQueue;
*		MyWorkQueue.Enqueue([]() { printf("Lambda function is called!\n"); };
*
*		while (MyWorkQueue.HaveMoreWork()) {
*			MyWorkQueue.Dequeue();
*		}
*
*		WorkQueueBase<bool> TaskList;
*
*		while (TaskList.HaveMoreWork()) {
*			if (TaskList.RunWork()) {
*				MyWorkQueue.Pop();
*			}
*		}
*
*		return 0;
*	}
* @endcode
* �o��:
* @code
*		SomeFunction is called!
*		Lambda function is called!
*		Mytask : 1
*		Mytask : 2
*		Mytask : 3
*		Mytask : 4
*		Mytask : 5
*		Mytask : 6
*		Mytask : 7
*		Mytask : 8
*		Mytask : 9
*		Mytask : 10
* @endcode
* �ŏI�X�V:
*		2016/07/22
* --------------------------------------------
*/
#include <functional>
#include <queue>

namespace Utility {
	
	template <class RetType, class ...ArgTypes>
	/// <summary>�֐������X�g�Ǘ����A���s�ł���悤�ɂ��܂��B</summary>
	/// <param name="RetType">�֐��̖߂�l�̌^</param>
	/// <param name="ArgTypes">�֐��̈����̌^�B�����w�肪�ł��܂��B</param>
	class WorkQueueBase {
	public:
		// ���̃N���X�������֐��̌^
		typedef std::function<RetType(ArgTypes...)> MyFunction;
	private:
		// �֐��̃��X�g
		std::queue<MyFunction> MyQueue;

		// �R�s�[�͋֎~
		WorkQueueBase(const WorkQueueBase&) = delete;
		WorkQueueBase &operator=(const WorkQueueBase&) = delete;
	public:
		
		/// <summary>�V���� WorkQueueBase �I�u�W�F�N�g�����������܂��B</summary>
		WorkQueueBase() = default;

		/// <summary>���s���ׂ��֐����܂��c���Ă��邩�ǂ����B</summary>
		/// <returns>�֐����c���Ă���� true, �����łȂ���� false</returns>
		bool HaveMoreWork() const {
			return (MyQueue.size() > 0);
		}

		/// <summary>���s���ׂ��֐���ǉ����܂��B</summary>
		/// <param name="work">�ǉ�����֐��ւ̎Q��</param>
		void Enqueue(MyFunction &work) {
			MyQueue.push(work);
		}

		/// <summary>���s���ׂ��֐���ǉ����܂��B</summary>
		/// <param name="work">�ǉ�����֐��ւ̎Q��</param>
		void Enqueue(MyFunction &&work) {
			MyQueue.push(work);
		}

		/// <summary>��Ԑ擪�ɂ���֐������s���A�L���[����폜���܂��B</summary>
		RetType Dequeue(ArgTypes... args) {
			MyQueue.front()(args...);
			MyQueue.pop();
		}

		/// <summary>��Ԑ擪�ɂ���֐������s���܂��B</summary>
		RetType RunWork(ArgTypes... args) {
			MyQueue.front()(args...);
		}

		/// <summary>��Ԑ擪�ɂ���֐����L���[����폜���܂��B</summary>
		void Pop() {
			MyQueue.pop();
		}
	};

	/// <summary>�֐������X�g�Ǘ����A���s�ł���悤�ɂ��܂��B</summary>
	typedef WorkQueueBase<void> WorkQueue;
}