#include "SceneBase.h"
#include "DxLib.h"
#include "InputManager.h"
#include "GameResourseManager.h"
#include "SceneFader.h"

// ゲーム実演画面
class ShutterScene : public Scene {
	int Flames;
	int grHandle;
	int musHandle;

	int Width, Height;

	ShutterRenderer Shut;
public:
	ShutterScene();
	~ShutterScene();

	bool Initialize() override;
	void Update() override;
	void Draw() override;
};



CreateScene(ShutterScene);

ShutterScene::ShutterScene() {
	grHandle = ScreenCapture();
	musHandle = LoadSoundMem("sei_ge_garasu_kudake02.mp3");
	GetDrawScreenSize(&Width, &Height);
}

ShutterScene::~ShutterScene() {
	DeleteGraph(grHandle);
}

bool ShutterScene::Initialize() {
	Shut.Initialize(Width, Height);

	return true;
}

void ShutterScene::Update() {
	Shut.Update();

	if (Shut.EffectFinished()) {
		Shut.Reset();
		PlaySoundMem(musHandle, DX_PLAYTYPE_BACK);
	}

	if (InputManager::Instance().KeyPressedOnce(KEY_INPUT_Z)) {
		OnSceneChange(SceneType::ST_WaveTest);
	}
}

void ShutterScene::Draw() {
	DrawGraph(0, 0, grHandle, FALSE);
	Shut.Apply();

	DrawString(Width - 380, Height - 23, "画面が割れて落ちていくエフェクトプログラム", 0xffffff);
}