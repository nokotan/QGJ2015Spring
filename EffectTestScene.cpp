#include "SceneBase.h"
#include "DxLib.h"
#include "InputManager.h"
#include "GameResourseManager.h"
#include "Main.h"
#include "WorkQueueClass.h"
#include "SceneFader.h"

#include <string>
#include <map>

// �I�[�v�j���O
class EffectTestScene : public Scene {
	int Flames;
	int GrHandle, Ozi;
	int Width, Height;

	StencilBuffer Mask;
	ImageMaskRenderer Shadow;
	MosaicRenderer Mosaic;
	ImageMosaicRenderer ImageMosaic;
	WaveEffectRenderer WaveEffect;
public:
	EffectTestScene();
	~EffectTestScene();

	bool Initialize() override;
	void Update() override;
	void Draw() override;
};



// ������폜���Ȃ��ł��������B
CreateScene(EffectTestScene);

EffectTestScene::EffectTestScene() {
	GetDrawScreenSize(&Width, &Height);
}

EffectTestScene::~EffectTestScene() {
	WaveEffect.Dispose();
	ImageMosaic.Dispose();
	Mosaic.Dispose();
	Shadow.Dispose();
	Mask.Dispose();
}

bool EffectTestScene::Initialize() {
	GrHandle = LoadGraph("background_game.png");
	Ozi = LoadGraph("������Ղف[o-zi.png");

	Mask.Initialize(Width, Height);
	Shadow.Initialize();
	Mosaic.Initialize(Width, Height);
	ImageMosaic.Initialize();
	WaveEffect.Initialize();

	return true;
}

void EffectTestScene::Update() {
	WaveEffect.Update();

	if (InputManager::Instance().KeyPressedOnce(KEY_INPUT_Z)) {
		OnSceneChange(SceneType::ST_Shutter);
	}
}

void EffectTestScene::Draw() {
	static int i = 150, k = 150, j = 64;
	static int tick = 0;
	tick++;

	if (InputManager::Instance().KeyPressed(KEY_INPUT_X)) {
		if (i > 0)	i--;
	}

	if (InputManager::Instance().KeyPressed(KEY_INPUT_C)) {
		if (i < 150)	i++;
	}

	if (InputManager::Instance().KeyPressed(KEY_INPUT_A)) {
		if (k > 0)	k--;
	}

	if (InputManager::Instance().KeyPressed(KEY_INPUT_S)) {
		if (k < 150)	k++;
	}

	if (InputManager::Instance().KeyPressed(KEY_INPUT_Q)) {
		if (j > 0)	j--;
	}

	if (InputManager::Instance().KeyPressed(KEY_INPUT_W)) {
		if (j < 64)	j++;
	}

	DrawBox(0, 0, Width, Height, GetColor(128, 128, 255), TRUE);

	DrawGraph(20, 260, GrHandle, TRUE);

	// �g�̃G�t�F�N�g
	WaveEffect.Apply();
	DrawString(20, 240, "WaveEffect �̃e�X�g", 0xffffff);
	
	// �e
	Shadow.DrawImage(745, 65, Ozi, GetColorU8(0, 0, 0, 0), GetColorU8(0, 0, 0, 255));

	// ���U�C�N�摜
	DrawString(740, 40, "�e�ƃ��U�C�N�摜�̃e�X�g", 0xffffff);
	ImageMosaic.DrawImage(740, 60, 64 / (65 - j), 64 / (65 - j), Ozi);

	// TriangleGraph �̃e�X�g
	DrawString(20, 40, "DrawTriangleGraph �̃e�X�g", 0xffffff);
	DrawTriangleGraph(20, 60, 0, 0, 640, 100, 150, 150, GrHandle, TRUE);
	DrawTriangle(20, 260, 660, 360, 170, 410, GetColor(255, 0, 0), FALSE);

	// �}�X�N���
	DrawString(740, 240, "�}�X�N��ʁA���U�C�N�̃e�X�g�͉�ʑS�̂ōs���܂��B", 0xffffff);
	Mask.AddRectangle(0, 0, Width, Height, GetColor(0, 0, 0));
	int Length = Width * i / 80;
	Mask.AddExtendedImage(Width / 2 - Length, Height / 2 - Length, Width / 2 + Length, Height / 2 + Length, Ozi);
	int Radius = i * Height / 100;
	Mask.AddCircle(Width / 2, Height / 2, Radius, GetColor(255, 255, 255));

	Mask.Apply();
	
	// ���U�C�N
	Mosaic.Apply(Width / (151 - k), Height / (151 - k));

	// DrawString(3, 3, "EffectTest!", GetColor(255, 255, 255));

	DrawString(Width - 300, Height - 63, "[X][C]�Ń}�X�N��ʂ̑傫����ύX", 0xffffff);
	DrawString(Width - 310, Height - 43, "[A][S]�Ń��U�C�N��ʂ̉𑜓x��ύX", 0xffffff);
	DrawString(Width - 300, Height - 23, "[Q][W]�ō���̉��q�̉𑜓x��ύX", 0xffffff);
}