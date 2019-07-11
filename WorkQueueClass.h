#pragma once

/** @file WorkQueueClass.h 
* WorkQueueClass.h --------------------------
* 関数をリスト管理し、実行できるようにします。
* 名前空間:
*		Utility
* 使用例:
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
* 出力:
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
* 最終更新:
*		2016/07/22
* --------------------------------------------
*/
#include <functional>
#include <queue>

namespace Utility {
	
	template <class RetType, class ...ArgTypes>
	/// <summary>関数をリスト管理し、実行できるようにします。</summary>
	/// <param name="RetType">関数の戻り値の型</param>
	/// <param name="ArgTypes">関数の引数の型。複数指定ができます。</param>
	class WorkQueueBase {
	public:
		// このクラスが扱う関数の型
		typedef std::function<RetType(ArgTypes...)> MyFunction;
	private:
		// 関数のリスト
		std::queue<MyFunction> MyQueue;

		// コピーは禁止
		WorkQueueBase(const WorkQueueBase&) = delete;
		WorkQueueBase &operator=(const WorkQueueBase&) = delete;
	public:
		
		/// <summary>新しい WorkQueueBase オブジェクトを初期化します。</summary>
		WorkQueueBase() = default;

		/// <summary>実行すべき関数がまだ残っているかどうか。</summary>
		/// <returns>関数が残っていれば true, そうでなければ false</returns>
		bool HaveMoreWork() const {
			return (MyQueue.size() > 0);
		}

		/// <summary>実行すべき関数を追加します。</summary>
		/// <param name="work">追加する関数への参照</param>
		void Enqueue(MyFunction &work) {
			MyQueue.push(work);
		}

		/// <summary>実行すべき関数を追加します。</summary>
		/// <param name="work">追加する関数への参照</param>
		void Enqueue(MyFunction &&work) {
			MyQueue.push(work);
		}

		/// <summary>一番先頭にある関数を実行し、キューから削除します。</summary>
		RetType Dequeue(ArgTypes... args) {
			MyQueue.front()(args...);
			MyQueue.pop();
		}

		/// <summary>一番先頭にある関数を実行します。</summary>
		RetType RunWork(ArgTypes... args) {
			MyQueue.front()(args...);
		}

		/// <summary>一番先頭にある関数をキューから削除します。</summary>
		void Pop() {
			MyQueue.pop();
		}
	};

	/// <summary>関数をリスト管理し、実行できるようにします。</summary>
	typedef WorkQueueBase<void> WorkQueue;
}