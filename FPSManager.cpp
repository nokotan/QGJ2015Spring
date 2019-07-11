#include "FPSManager.h"
#include "DxLib.h"

namespace Utility {

	FPSManager::FPSManager(int targetfps) :
		m_StartTime(0),
		m_AverageFPS(static_cast<float>(targetfps)),
		m_TargetProcessTime(1000 / targetfps) {
	}

	int FPSManager::SetFPS(int val) {
		m_AverageFPS = static_cast<float>(val);
		m_TargetProcessTime = 1000 / val;
		return val;
	}

	float FPSManager::GetAverageFPS() const {
		return m_AverageFPS;
	}

	void FPSManager::Update() {
		int CurrentTime = GetNowCount();
		m_AverageFPS = m_AverageFPS * .99f + 10.f / (CurrentTime - m_PreviousTime);
		m_PreviousTime = CurrentTime;
	}

	void FPSManager::Wait() {
		int WaitTime = m_TargetProcessTime - GetNowCount() + m_StartTime;

		if (WaitTime > 0) {
			WaitTimer(WaitTime);
		}

		m_StartTime = GetNowCount();
	}

}