#pragma once

/**
* @file FPSManager.h
* FPSManager.h -----------------------------------------
* 実行速度を監視し、ゲームを任意のフレームレートで実行できるようにします。
* 最終更新:
*		2016/08/12
* ------------------------------------------------------
*/

namespace Utility {

	///	<summary>実行速度を監視し、ゲームのフレームレートを調整します。</summary>
	class FPSManager {
		int m_StartTime;
		int m_PreviousTime;
		int m_TargetProcessTime;
		float m_AverageFPS;
	public:
		/// <summary>新しい FPSManager オブジェクトを初期化します。</summary>
		/// <param name="target_fps">目標とするフレームレート。既定では、60FPSが指定されます。</param>
		FPSManager(int target_fps = 60);

		/// <summary>フレームレートを設定します。</summary>
		/// <param name="target_fps">目標とするフレームレート</param>		
		int SetFPS(int target_fps);
		
		/// <summary>平均フレームレートを取得します。</summary>
		/// <returns>直近の平均フレームレート</returns>
		float GetAverageFPS() const;

		/// <summary>
		/// 平均フレームレートを更新します。ゲームループが進むたびに呼び出してください。
		/// </summary>
		void Update();
		
		/// <summary>
		/// 設定したフレームレートになるように待機します。
		/// 現在のフレームレートが設定したフレームレートより遅れている場合、すぐに処理を返します。
		/// </summary>
		void Wait();
	};

}