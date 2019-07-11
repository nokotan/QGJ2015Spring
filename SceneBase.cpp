#include "SceneBase.h"

using namespace std;

#pragma region Scene

Scene::Scene() {
}

Scene::~Scene() {

}

bool Scene::Initialize() {
	return true;
}

#pragma region Event

void Scene::OnSceneChange(SceneType val) {
	if (scenechange != nullptr)
		scenechange(val);
}

void Scene::AddHandler(function<void(SceneType)> funcptr) {
	scenechange = funcptr;
}

void Scene::RemoveHandler() {
	scenechange = nullptr;
}

#pragma endregion

#pragma endregion