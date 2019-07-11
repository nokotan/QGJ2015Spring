#include "SceneBase.h"
#include "DxLib.h"
#include "InputManager.h"
#include "GameResourseManager.h"
#include "SceneFader.h"

// 操作説明画面
class WaveTestScene : public Scene {
	int Flames;
	int grHandle;
	int mushandle[2];
	int Width, Height;

	WaveEffect2Renderer Wave[5];
public:
	WaveTestScene();
	~WaveTestScene();

	bool Initialize() override;
	void Update() override;
	void Draw() override;
};



CreateScene(WaveTestScene);

WaveTestScene::WaveTestScene() {
	grHandle = ScreenCapture();
	mushandle[0] = LoadSoundMem("sei_ge_suiteki_reverb01.mp3");
	mushandle[1] = LoadSoundMem("sei_ge_suiteki_reverb03.mp3");
	GetDrawScreenSize(&Width, &Height);
}

WaveTestScene::~WaveTestScene() {
	DeleteGraph(grHandle);
}

bool WaveTestScene::Initialize() {
	for (auto& wave : Wave) {
		wave.Initialize();
		wave.Decay = 1200.f;
	}

	return true;
}

void WaveTestScene::Update() {
	int i = 0;

	for (auto& wave : Wave) {
		if (Flames > i * 120)
			wave.Update();

		if ((Flames + i * 120) % 600 == 0) {
			PlaySoundMem(mushandle[GetRand(1)], DX_PLAYTYPE_BACK);
			wave.Reset();
			wave.SetCenterPos(GetRand(Width - 1), GetRand(Height - 1));
		}

		i++;
	}

	Flames++;
	
	if (InputManager::Instance().KeyPressedOnce(KEY_INPUT_Z)) {
		OnSceneChange(SceneType::ST_EffectTest);
	}
}

void WaveTestScene::Draw() {
	DrawGraph(0, 0, grHandle, FALSE);

	for (auto& wave : Wave) {
		wave.Apply();
	}

	DrawString(Width - 300, Height - 23, "画面に波紋を描くシェーダプログラム", 0xffffff);
}