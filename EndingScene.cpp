#include "SceneBase.h"
#include "DxLib.h"
#include "InputManager.h"
#include "GameResourseManager.h"

// エンディング画面
class EndingScene : public Scene {
	int Flames;
public:
	EndingScene();
	~EndingScene();

	bool Initialize() override;
	void Update() override;
	void Draw() override;
};



CreateScene(EndingScene);

EndingScene::EndingScene() {

}

EndingScene::~EndingScene() {

}

bool EndingScene::Initialize() {
	return true;
}

void EndingScene::Update() {

}

void EndingScene::Draw() {

}