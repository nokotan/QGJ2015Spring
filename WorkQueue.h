#pragma once

/** @file WorkQueue.h
* WorkQueue.h --------------------------------
* ��������і߂�l�̂Ȃ��֐������X�g�Ǘ����A���s�ł���悤�ɂ��܂��B
*
* �g�p��:
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
* �o��:
* @code
*		SomeFunction is called!
*		Lambda function is called!
* @endcode
* �ŏI�X�V�F
*		2016/08/12
* --------------------------------------------
*/

#include <functional>

namespace Utility {

	/// <summary>���s���ׂ��֐����܂��c���Ă��邩�ǂ����B</summary>
	/// <returns>�֐����c���Ă���� true, �����łȂ���� false</returns>
	bool WorkQueueHaveMoreWork();

	/// <summary>���s���ׂ��֐���ǉ����܂��B</summary>
	/// <param name="work">�ǉ�����֐��ւ̎Q��</param>
	void WorkQueueEnqueue(std::function<void()> &work);

	/// <summary>���s���ׂ��֐���ǉ����܂��B</summary>
	/// <param name="work">�ǉ�����֐��ւ̎Q��</param>
	void WorkQueueEnqueue(std::function<void()> &&work);

	/// <summary>��Ԑ擪�ɂ���֐������s���A�L���[����폜���܂��B</summary>
	void WorkQueueDequeue();

	/// <summary>��Ԑ擪�ɂ���֐������s���܂��B</summary>
	void WorkQueueRunWork();

	/// <summary>��Ԑ擪�ɂ���֐����L���[����폜���܂��B</summary>
	void WorkQueuePop();

}

using namespace Utility;
