#include "SceneBase.h"
#include "InputManager.h"
#include "GameResourseManager.h"
#include "DxLib.h"
#include "WorkQueueClass.h"
#include "Main.h"
#include "SceneFader.h"
#include "FPSManager.h"

#include <time.h>
#include <crtdbg.h>
#include <functional>

// エントリポイント
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int);
// ループで必ず行う処理
int ProcessLoop();

#pragma region 割り込み関係

// 割り込み関数のリスト
static Utility::WorkQueue MyQueue;

void AddWork(std::function<void()> func) {
	MyQueue.Enqueue(std::move(func));
}

void AddWorkRef(std::function<void()>& func) {
	MyQueue.Enqueue(func);
}

void AddWorkRef(std::function<void()>&& func) {
	MyQueue.Enqueue(std::move(func));
}

#pragma endregion

// もし、デバッグビルドなら...
#if defined(_DEBUG)
	
int _cdecl main() {
	return WinMain(GetModuleHandle(0), 0, 0, SW_SHOWDEFAULT);
}

#endif

int DrawTriangleGraph(int x, int y, int srcx1, int srcy1, int srcx2, int srcy2, int srcx3, int srcy3, int grhandle, int transflag) {
	static VERTEX vertex[3];

	int width, height;
	GetGraphSize(grhandle, &width, &height);
	
	vertex[0].a = vertex[0].r = vertex[0].g = vertex[0].b = 255;
	vertex[1].a = vertex[1].r = vertex[1].g = vertex[1].b = 255;
	vertex[2].a = vertex[2].r = vertex[2].g = vertex[2].b = 255;

	vertex[0].x = (float)x;
	vertex[1].x = (float)(x + srcx2 - srcx1);
	vertex[2].x = (float)(x + srcx3 - srcx1);

	vertex[0].y = (float)y;
	vertex[1].y = (float)(y + srcy2 - srcy1);
	vertex[2].y = (float)(y + srcy3 - srcy1);

	vertex[0].u = (float)srcx1 / width;
	vertex[1].u = (float)srcx2 / width;
	vertex[2].u = (float)srcx3 / width;

	vertex[0].v = (float)srcy1 / height;
	vertex[1].v = (float)srcy2 / height;
	vertex[2].v = (float)srcy3 / height;
	
	return DrawPolygon(vertex, 1, grhandle, transflag, TRUE);
}

static int ScreenHandle = DX_NONE_GRAPH;

int DrawGlowString(int x, int y, const char text[], unsigned int color, unsigned int glowcolor) {

#ifdef _DEBUG
	if (ScreenHandle < 0) return -1;
#endif

	int Width, Height;
	GetGraphSize(ScreenHandle, &Width, &Height);
	
	int CurrentDrawScreen = GetDrawScreen();
	SetDrawScreen(ScreenHandle);

	// DrawBox(0, 0, Width, Height, 0xffffff, TRUE);
	ClearDrawScreen();
	DrawString(x, y, text, glowcolor);
	SetDrawScreen(CurrentDrawScreen);

	GraphFilter(ScreenHandle, DX_GRAPH_FILTER_GAUSS, 8, 6400);
	DrawGraph(0, 0, ScreenHandle, TRUE);
	DrawString(x, y, text, color);

	return 0;
}

#include "DxLib.h"

#include "Collider2D.h"

struct Player {
	int x, y, width, height, dx, dy;
};

# if 0

#include "Box2D/Box2D.h"

#if defined(_DEBUG)
#	pragma comment(lib, "Box2D.lib")
#endif

void Box2DSample() {
	b2Vec2 Gravity { 0.0f, 9.8f };
	std::shared_ptr<b2World> World { new b2World(Gravity) };
	std::vector<b2Body*> BodyList;

	b2BodyDef FloorBody;
	FloorBody.type = b2BodyType::b2_kinematicBody;
	FloorBody.linearVelocity.Set(0.5f, 0.0f);
	FloorBody.position.Set(2.5f, 3.0f);
	
	b2PolygonShape FloorShape;
	FloorShape.SetAsBox(2.0f, 0.1f);

	b2Body* Floor = World->CreateBody(&FloorBody);
	Floor->CreateFixture(&FloorShape, 1.0f);
	
	b2BodyDef BoxBody;
	BoxBody.type = b2BodyType::b2_dynamicBody;
	BoxBody.position.Set(2.5f, 0.0f);

	//BoxBody.fixedRotation = true;

	b2PolygonShape BoxShape;
	BoxShape.SetAsBox(0.5f, 0.5f);
	
	b2FixtureDef BoxFixture;
	BoxFixture.shape = &BoxShape;
	BoxFixture.density = 1.0f;
	BoxFixture.friction = 1.0f;

	b2Body* Box = World->CreateBody(&BoxBody);
	Box->CreateFixture(&BoxShape, 1.0f);
	Box->CreateFixture(&BoxFixture);

	int Flames = 0;

	while (ProcessMessage() != -1 && ClearDrawScreen() != -1) {
		InputManager::Instance().Update();

		if (InputManager::Instance().KeyPressedOnce(KEY_INPUT_SPACE)) {
			Box->ApplyForceToCenter(b2Vec2(0.0f, -1000.0f), true);
		}

		//if (InputManager::Instance().KeyPressedOnce(KEY_INPUT_LEFT)) {
		//	Floor->SetLinearVelocity(b2Vec2(-0.5f, 0.0f));
		//}
		//
		//if (InputManager::Instance().KeyPressedOnce(KEY_INPUT_RIGHT)) {
		//	Floor->SetLinearVelocity(b2Vec2(0.5f, 0.0f));
		//}

		Floor->SetLinearVelocity(b2Vec2(0.5f * cos(Flames * DX_PI_F / 360), 0.5f * sin(Flames * DX_PI_F / 360)));
		Flames++;

		World->Step(1.0f / 60, 10, 3);

		clsDx();
		printfDx("%f, %f", Box->GetPosition().x, Box->GetPosition().y); // player.x, player.y);

		auto FloorPosition = Floor->GetPosition();
		FloorPosition *= 100.0f;

		DrawBox(FloorPosition.x - 200, FloorPosition.y - 10, FloorPosition.x + 200, FloorPosition.y + 10, GetColor(0, 216, 0), TRUE);
		
		auto BoxPosition = Box->GetPosition();
		BoxPosition *= 100.0f;
		
		DrawBox(BoxPosition.x - 50, BoxPosition.y - 50, BoxPosition.x + 51, BoxPosition.y + 51, GetColor(255, 255, 255), TRUE);
		ScreenFlip();
	};
}

# endif

int WINAPI WinMain_(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1) {
		return 0;
	}

	SetBackgroundColor(16, 0, 16);
	SetDrawScreen(DX_SCREEN_BACK);

	//Box2DSample();
	//DxLib_End();
	//return 0;

	// タイルマップとして使う２次元配列
	const int MapTilesWidth = 20;
	const int MapTilesHeight = 15;
	int MapTiles [MapTilesWidth][MapTilesHeight];

	// タイルマップを -1 (何もない) で埋める
	for (int i = 0; i < MapTilesWidth; i++) {
		for (int j = 0; j < MapTilesHeight; j++) {
			MapTiles[i][j] = -1;

			// 一番下は床として 0 を入れておく
			if (j == MapTilesHeight - 1) {
				MapTiles[i][j] = 0;
			}
		}
	}
	TemporaryMap<int, 20, 15> TempMapTiles { MapTiles };

	MapTiles[3][13] = 0;
	struct Player player;
	// player を初期化
	player.x = 0;
	player.y = 384;
	player.width = 32;
	player.height = 64;
	player.dx = 0;
	player.dy = 0;

	CMap MyMap { 6, 1 };
	MyMap.Fill(0);
	MyMap.y = 12 * 32;
	MyMap.dx = 2;
	MyMap.dy = -2;

	CMap MyBlock { 4, 1 };
	MyBlock.Fill(0);
	MyBlock.x = 32;
	MyBlock.y = 0;
	MyBlock.dx = 0;
	MyBlock.dy = 0;

	CMap MyBlock2 { 4, 1 };
	MyBlock2.Fill(0);
	MyBlock2.x = 32 * 3;
	MyBlock2.y = -32 * 1;
	MyBlock2.dx = 0;
	MyBlock2.dy = 0;

	CMap MyBlock3{ 4, 1 };
	MyBlock3.Fill(0);
	MyBlock3.x = 32 * 6;
	MyBlock3.y = -32 * 2;
	MyBlock3.dx = 0;
	MyBlock3.dy = 0;

	CMap MyBlock4{ 4, 1 };
	MyBlock4.Fill(0);
	MyBlock4.x = 32 * 6;
	MyBlock4.y = -32 * 3;
	MyBlock4.dx = 0;
	MyBlock4.dy = 0;

	std::vector<IMapTileLayor<int>*> Layors{ &TempMapTiles, &MyBlock4, &MyBlock3, &MyBlock2, &MyBlock, &MyMap};
	std::vector<CMap*> MyLayors { &MyBlock, &MyBlock2, &MyBlock3, &MyBlock4 };

	bool CanJump = false;

	// メインループ
	while (ProcessMessage() != -1 && ClearDrawScreen() != -1) {
		clsDx();
		printfDx("%f, %f", player.x, player.y);

		// 入力に応じて、プレイヤーのスピードを変える
		if (CheckHitKey(KEY_INPUT_LEFT)) {
			player.dx = -2;
		}
		else if (CheckHitKey(KEY_INPUT_RIGHT)) {
			player.dx = 2;
		}
		else {
			player.dx = 0;
		}

		if (CheckHitKey(KEY_INPUT_SPACE) && CanJump) {
			player.dy = -20;
		}

		// 重力加速度を加える。
		player.dy += 1;
		if (player.dy > 10) {
			player.dy = 10;
		}

		for (CMap* Layor : MyLayors) {
			Layor->dx = 0;
			Layor->dy += 1;
			if (Layor->dy > 10) {
				Layor->dy = 10;
			}
		}
		if (MyMap.x <= 0) {
			MyMap.dx = 2;
			MyMap.dy = -2;
		} else if (MyMap.x >= 6 * 32) {
			MyMap.dx = -2;
			MyMap.dy = 2;
		}

		//MyBlock.dx = MyMap.dx;

		MyMap.x += MyMap.dx;
		MyMap.y += MyMap.dy;
		for (CMap* Layor : MyLayors) {
			Collider2D::CollisionCheck(*Layor, Layors);
		}

		CanJump = (Collider2D::CollisionCheck(player, Layors) & Direction::Down) != 0;

		// プレイヤー描画
		DrawBox(player.x, player.y, player.x + player.width, player.y + player.height, GetColor(255, 255, 255), TRUE);
		for (int i = 0; i < MapTilesWidth; i++) {
			for (int j = 0; j < MapTilesHeight; j++) {
				if (MapTiles[i][j] != -1) {
					DrawBox(i * 32, j * 32, i * 32 + 32, j * 32 + 32, GetColor(0, 216, 0), TRUE);
				}
			}
		}

		for (int i = 0; i < MyMap.Cols(); i++) {
			DrawBox(i * 32 + MyMap.x, MyMap.y, i * 32 + MyMap.x + 32, MyMap.y + 32, GetColor(0, 0, 216), TRUE);
		}

		for (int i = 0; i < MyBlock.Cols(); i++) {
			DrawBox(i * 32 + MyBlock.x, MyBlock.y, i * 32 + MyBlock.x + 32, MyBlock.y + 32, GetColor(216, 0, 0), TRUE);
		}

		for (int i = 0; i < MyBlock2.Cols(); i++) {
			DrawBox(i * 32 + MyBlock2.x, MyBlock2.y, i * 32 + MyBlock2.x + 32, MyBlock2.y + 32, GetColor(216, 216, 0), TRUE);
		}

		for (int i = 0; i < MyBlock3.Cols(); i++) {
			DrawBox(i * 32 + MyBlock3.x, MyBlock3.y, i * 32 + MyBlock3.x + 32, MyBlock3.y + 32, GetColor(216, 0, 216), TRUE);
		}

		for (int i = 0; i < MyBlock4.Cols(); i++) {
			DrawBox(i * 32 + MyBlock4.x, MyBlock4.y, i * 32 + MyBlock4.x + 32, MyBlock4.y + 32, GetColor(0, 216, 216), TRUE);
		}

		ScreenFlip();
	}

	DxLib::DxLib_End();
	return 0;
}

#include <iostream>
#include <new>

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	SRand((unsigned)time(NULL));

#if defined(_DEBUG)
	ChangeWindowMode(TRUE); 
#else
	SetGraphMode(1366, 768, 32);
#endif
	
	SetDoubleStartValidFlag(TRUE);
	SetAlwaysRunFlag(TRUE);

	SetMainWindowText("QGJ 2015 Spring");

	SetUse3DFlag(TRUE);
	SetUseDirect3DVersion(DX_DIRECT3D_9EX);

	if (DxLib_Init() == -1) return -1;

	SetDrawScreen(DX_SCREEN_BACK);
	SetBackgroundColor(16, 0, 16);

	SceneManager Manager(SceneType::ST_Reflection);
	// SceneManager Manager(SceneType::ST_EffectTest);
	Utility::FPSManager FPS;
	
	GameResourseMgr.InitializeImages();
	GameResourseMgr.InitializeMusics();

	while (ProcessLoop() == 0) {		
		if (MyQueue.HaveMoreWork()) {
			// 割り込み操作を実行
			MyQueue.Dequeue();
		}
	
		InputMgr.Update();
		FPS.Update();

		Manager.Update();
		Manager.Draw();

		ScreenFlip();
	}

	GameResourseMgr.DisposeImages();
	GameResourseMgr.DisposeMusics();

	DxLib_End();

	return 0;
}

static int ProcessLoop() {
	if (ProcessMessage() != 0) return -1;
	if (ClearDrawScreen() != 0) return -1;
	return 0;
}
