#include "SceneBase.h"
#include "DxLib.h"
#include "InputManager.h"
#include "GameResourseManager.h"
#include "Main.h"

// ÉQÅ[ÉÄâÊñ 
class OpeningScene : public Scene {
	int Box;
public:
	OpeningScene();
	OpeningScene(int, int) {};
	~OpeningScene();

	bool Initialize() override;
	void Update() override;
	void Draw() override;
};



CreateScene(OpeningScene);

template <>
Scene *Scene::CreateInstance<OpeningScene>(int x, int y) {
	return new OpeningScene(x, y);
}

template Scene *Scene::CreateInstance<OpeningScene>(int, int);

OpeningScene::OpeningScene() {
}

OpeningScene::~OpeningScene() {
}

bool OpeningScene::Initialize() {
	Box = MV1LoadModel("Box.x");
	MV1SetPosition(Box, VGet(320.f, 240.f, 2.f));
	MV1SetScale(Box, VGet(32.f, 32.f, 32.f));
	return true;
}

void OpeningScene::Update() {
	
	if (InputManager::Instance().KeyPressedOnce(KEY_INPUT_ESCAPE))
		OnSceneChange(SceneType::ST_Opening);
}

void OpeningScene::Draw() {
	// DrawString(3, 3, "Opening!", 0xffffff);
	DrawGlowString(3, 3, "Opening", 0xffffff, 0xffffff);
	MV1DrawModel(Box);
}