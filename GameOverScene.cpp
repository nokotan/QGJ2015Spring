#include "SceneBase.h"
#include "DxLib.h"
#include "InputManager.h"
#include "GameResourseManager.h"

// �Q�[���I�[�o�[���
class GameOverScene : public Scene {
	int Flames;
public:
	GameOverScene();
	~GameOverScene();

	bool Initialize() override;
	void Update() override;
	void Draw() override;
};



CreateScene(GameOverScene);

GameOverScene::GameOverScene() {

}

GameOverScene::~GameOverScene() {

}

bool GameOverScene::Initialize() {
	return true;
}

void GameOverScene::Update() {

}

void GameOverScene::Draw() {

}