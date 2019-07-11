#include "SceneBase.h"
#include "DxLib.h"
#include "InputManager.h"
#include "GameResourseManager.h"
#include "Main.h"

// ÉQÅ[ÉÄâÊñ 
class LevelScene : public Scene {
	void OnSoundAdd(int musID);
public:
	LevelScene();
	~LevelScene();

	bool Initialize() override;
	void Update() override;
	void Draw() override;
};



CreateScene(LevelScene);

LevelScene::LevelScene() {
}

LevelScene::~LevelScene() {
}

bool LevelScene::Initialize() {
	return true;
}

void LevelScene::Update() {	
	if (InputManager::Instance().KeyPressedOnce(KEY_INPUT_ESCAPE))
		OnSceneChange(SceneType::ST_Opening);
}

void LevelScene::Draw() {
	DrawString(3, 3, "Level!", 0xffffff);
}