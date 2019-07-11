#include "SceneBase.h"
#include "InputManager.h"
#include "DebugHelper.h"
#include "DxLib.h"

// Ž–‘OéŒ¾
#define DeclearScenes(name) class name##Scene;

SceneTypes(DeclearScenes)

#undef DeclearScenes

using namespace std;

SceneManager::SceneManager(SceneType val) :
	CurrentScene((Scene*)0), 
	NextScene(val) {

	SceneChange();
}

SceneManager::SceneManager() :
	CurrentScene(nullptr),
	NextScene(ST_NULL) {
}

SceneManager::~SceneManager() {
	delete CurrentScene;
}



void SceneManager::Update() {
	if (NextScene != SceneType::ST_NULL) {
		SceneChange();
	}

	if (CurrentSceneType != SceneType::ST_Opening && InputMgr.GetResetRequestStatus()) {
		OnSceneChange(SceneType::ST_Opening);
	}

	if (CurrentScene != nullptr)
		CurrentScene->Update();
}

void SceneManager::Draw() const {
	if (CurrentScene != nullptr)
		CurrentScene->Draw();
}

#define SceneChangeDef(name)									\
	case SceneType::ST_##name:									\
		CurrentScene = Scene::CreateInstance<name##Scene>();	\
		break;

void SceneManager::SceneChange() {
	delete CurrentScene;

	switch (NextScene) {
		SceneTypes(SceneChangeDef)

		default:
			CurrentScene = (Scene*)0;

			DebugTrace("ArgumentException: The program will exit.");
			PostQuitMessage(0);
			break;
	}
	
	if (CurrentScene != nullptr) {
		CurrentScene->Initialize();
		CurrentScene->AddHandler(bind(&SceneManager::OnSceneChange, this, placeholders::_1));
	}

	CurrentSceneType = NextScene;
	NextScene = ST_NULL;
}

#undef SceneChangeDef

void SceneManager::OnSceneChange(SceneType val) {
	NextScene = val;
}