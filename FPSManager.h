#pragma once

/**
* @file FPSManager.h
* FPSManager.h -----------------------------------------
* ���s���x���Ď����A�Q�[����C�ӂ̃t���[�����[�g�Ŏ��s�ł���悤�ɂ��܂��B
* �ŏI�X�V:
*		2016/08/12
* ------------------------------------------------------
*/

namespace Utility {

	///	<summary>���s���x���Ď����A�Q�[���̃t���[�����[�g�𒲐����܂��B</summary>
	class FPSManager {
		int m_StartTime;
		int m_PreviousTime;
		int m_TargetProcessTime;
		float m_AverageFPS;
	public:
		/// <summary>�V���� FPSManager �I�u�W�F�N�g�����������܂��B</summary>
		/// <param name="target_fps">�ڕW�Ƃ���t���[�����[�g�B����ł́A60FPS���w�肳��܂��B</param>
		FPSManager(int target_fps = 60);

		/// <summary>�t���[�����[�g��ݒ肵�܂��B</summary>
		/// <param name="target_fps">�ڕW�Ƃ���t���[�����[�g</param>		
		int SetFPS(int target_fps);
		
		/// <summary>���σt���[�����[�g���擾���܂��B</summary>
		/// <returns>���߂̕��σt���[�����[�g</returns>
		float GetAverageFPS() const;

		/// <summary>
		/// ���σt���[�����[�g���X�V���܂��B�Q�[�����[�v���i�ނ��тɌĂяo���Ă��������B
		/// </summary>
		void Update();
		
		/// <summary>
		/// �ݒ肵���t���[�����[�g�ɂȂ�悤�ɑҋ@���܂��B
		/// ���݂̃t���[�����[�g���ݒ肵���t���[�����[�g���x��Ă���ꍇ�A�����ɏ�����Ԃ��܂��B
		/// </summary>
		void Wait();
	};

}