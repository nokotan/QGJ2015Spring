#pragma once

/** @file WorkQueue.h
* WorkQueue.h --------------------------------
* 引数および戻り値のない関数をリスト管理し、実行できるようにします。
*
* 使用例:
* @code
*	#include "WorkQueue.h"
*
*	void SomeFunction() {
*		printf("SomeFunction is called!\n");
*	}
*
*	int main() {
*		WorkQueueEnqueue(SomeFunction);
*		WorkQueueEnqueue([]() { printf("Lambda function is called!\n"); };
*
*		while (WorkQueueHaveMoreWork()) {
*			WorkQueueDequeue();
*		}
*		return 0;
*	}
* @endcode
* 出力:
* @code
*		SomeFunction is called!
*		Lambda function is called!
* @endcode
* 最終更新：
*		2016/08/12
* --------------------------------------------
*/

#include <functional>

namespace Utility {

	/// <summary>実行すべき関数がまだ残っているかどうか。</summary>
	/// <returns>関数が残っていれば true, そうでなければ false</returns>
	bool WorkQueueHaveMoreWork();

	/// <summary>実行すべき関数を追加します。</summary>
	/// <param name="work">追加する関数への参照</param>
	void WorkQueueEnqueue(std::function<void()> &work);

	/// <summary>実行すべき関数を追加します。</summary>
	/// <param name="work">追加する関数への参照</param>
	void WorkQueueEnqueue(std::function<void()> &&work);

	/// <summary>一番先頭にある関数を実行し、キューから削除します。</summary>
	void WorkQueueDequeue();

	/// <summary>一番先頭にある関数を実行します。</summary>
	void WorkQueueRunWork();

	/// <summary>一番先頭にある関数をキューから削除します。</summary>
	void WorkQueuePop();

}

using namespace Utility;
