#include "SceneBase.h"
#include "DxLib.h"
#include "InputManager.h"
#include "GameResourseManager.h"
#include "ReflectorHelper.hpp"

#define CDbl static_cast<double>
#define CInt static_cast<int>
#define CSng static_cast<float>



using namespace Reflection;

// ÉQÅ[ÉÄé¿âââÊñ 
class ReflectionScene : public Scene {
	int u; float v; double w; long s; int *x;
	int t[30];
	
	int Foo(int *X, const double Y, int Z) {
		*X = Y + Z;
		v = Y + Z;
		return *X + Y + Z;
	}

	double &GetVal(int x) {
		return w;
	}

	float Fizz(double X, int Y) {
		return X - Y;
	}

	double Buzz(float X) {
		return X * 10.f;
	}

	class Type : public TypeImpl<ReflectionScene> {
	public:
		Type() {;
			AddMemberTable(Initialize);
			AddMemberTable(Update);
			AddMemberTable(Draw);
			AddMemberTable(Foo);
			AddMemberTable(GetVal);
			AddMemberTable(Fizz);
			AddMemberTable(Buzz);

			AddMemberTable(OnSceneChange);
			AddFunction(typeid(Scene).name(), static_cast<Scene&(*)(ReflectionScene&)>([](ReflectionScene& obj) -> Scene& { return static_cast<Scene&>(obj); }));

			AddMemberTable(u);
			AddMemberTable(v);
			AddMemberTable(w);
			AddMemberTable(s);
			AddMemberTable(t);
			AddMemberTable(x);
		}
	} MyType;
public:
	ReflectionScene();
	~ReflectionScene();

	TypeBase &GetType() {
		return MyType;
	}

	bool Initialize() override;
	void Update() override;
	void Draw() override;
};



CreateScene(ReflectionScene);

#include <iostream>

ReflectionScene::ReflectionScene() {
	x = &u;
}

ReflectionScene::~ReflectionScene() {

}

bool ReflectionScene::Initialize() {
	Reflection::RegisterType(typeid(*this), Type());
	return true;
}

#include <conio.h>

void ReflectionScene::Update() {

	if (_kbhit()) {
		printf("ïœêîñºÇì¸óÕÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB\n");
		char buf[1024] {};
		scanf_s("%[^\n]", buf, 1024);
		getchar();

		if (strlen(buf) > 0) {
			if (buf[0] != '?') {
				char name[1024] {};
				char subbuf1[1024] {};
				char subbuf2[1024] {};
				char subbuf3[1024] {};

				if (!strcmp(buf, "exit")) {
					PostQuitMessage(0);
				} else if (sscanf_s(buf, "%s = %s", name, 1024, subbuf1, 1024) == 2) {
					GetType().SetMemberValue(name, std::ref(*this), static_cast<const char*>(subbuf1));
					GetType().DumpMemberValue(name, std::ref(*this));
				} else if (sscanf_s(buf, "%[^(] ( %[^),] , %[^),] , %[^),] )", name, 1024, subbuf1, 1024, subbuf2, 1024, subbuf3, 1024) == 4) {
					ArgsList Args = { std::ref(*this), static_cast<const char*>(subbuf1), static_cast<const char*>(subbuf2), static_cast<const char*>(subbuf3) };
					printf("%s(%s, %s, %s) returned %s\n", name, subbuf1, subbuf2, subbuf3, GetType().InvokeMember(name, Args).Get<std::string>().c_str());
				} else if (sscanf_s(buf, "%[^(] ( %[^),] , %[^),] )", name, 1024, subbuf1, 1024, subbuf2, 1024) == 3) {
					ArgsList Args = { std::ref(*this), static_cast<const char*>(subbuf1), static_cast<const char*>(subbuf2) };
					printf("%s(%s, %s) returned %s\n", name, subbuf1, subbuf2, GetType().InvokeMember(name, Args).Get<std::string>().c_str());
				} else if (sscanf_s(buf, "%[^.].%[^(] ( %[^),] )", name, 1024, subbuf1, 1024, subbuf2, 1024) == 3) {
					auto Objj = GetType().GetMemberValue(name, std::ref(*this));
					ArgsList Args = { Objj, static_cast<const char*>(subbuf2) };
					printf("%s(%s) returned %s\n", name, subbuf1, Objj.GetType().InvokeMember(subbuf1, Args).Get<std::string>().c_str());
				}
			} else {
				GetType().DumpMemberValue(buf + 1, std::ref(*this));
			}
		}
	}
}

void ReflectionScene::Draw() {
	DrawBox(s, u, s + v, u + w, GetColor(255, 0, 0), TRUE);
	DrawBox(s, u + w + 10, s + v, u + 2 * w + 10, GetColor(0, 0, 255), TRUE);
	DrawBox(s + v + 10, u, s + 2 * v + 10, u + w, GetColor(0, 255, 0), TRUE);
	DrawBox(s + v + 10, u + w + 10, s + 2 * v + 10, u + 2 * w + 10, GetColor(255, 255, 0), TRUE);
}