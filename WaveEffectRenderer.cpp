#include "SceneFader.h"
#include <new>

WaveEffectRenderer::WaveEffectRenderer() :
	Flames(0),
	Amplitude(5.f), 
	Flequency(1.f),
	Lambda(64.f) {

	new(Index) int[6] { 0, 1, 3, 0, 3, 2 };

	VECTOR TexturePosition[4] =
	{ VGet(0.f, 0.f, 0.f), VGet(1.f, 0.f, 0.f), VGet(0.f, 1.f, 0.f), VGet(1.f, 1.f, 0.f) };

	for (int i = 0; i < 6; i++) {
		Vertexes[i].rhw = 1.f;
		Vertexes[i].u = TexturePosition[Index[i]].x;
		Vertexes[i].v = TexturePosition[Index[i]].y;
	}
}

bool WaveEffectRenderer::Initialize() {
	int GraphWidth, GraphHeight;
	GetGraphSize(DX_SCREEN_BACK, &GraphWidth, &GraphHeight);
	TempScreen = MakeScreen(GraphWidth, GraphHeight);
	return (pShader = LoadPixelShader("ShaderFiles/WaveEffectPixelShader.pso")) != -1;
}

void WaveEffectRenderer::Dispose() {
	DeleteShader(pShader);
	DeleteGraph(TempScreen);
}

void WaveEffectRenderer::Update() {
	Flames++;
}

void WaveEffectRenderer::DrawImage(int x, int y, int grhandle) {
	int GraphWidth, GraphHeight;
	GetGraphSize(grhandle, &GraphWidth, &GraphHeight);

	Position[0] = VGet(x + 1.f, y + 1.f, 0.f);
	Position[1] = VGet(float(x + GraphWidth), y + 1.f, 0.f);
	Position[2] = VGet(x + 1.f, float(y + GraphHeight), 0.f);
	Position[3] = VGet(float(x + GraphWidth), float(y + GraphHeight), 0.f);

	VECTOR TexturePosition[4] =
	{ VGet(0.f, 0.f, 0.f), VGet(.625f, 0.f, 0.f), VGet(0.f, .9375f, 0.f), VGet(.625f, .9375f, 0.f) };

	for (int i = 0; i < 6; i++) {
		Vertexes[i].rhw = 1.f;
		Vertexes[i].u = TexturePosition[Index[i]].x;
		Vertexes[i].v = TexturePosition[Index[i]].y;
	}

	for (int i = 0; i < 6; i++)
		Vertexes[i].pos = Position[Index[i]];

	GraphSize.x = float(GraphWidth);
	GraphSize.y = float(GraphHeight);
	GraphSize.z = 1.f / float(GraphWidth);
	GraphSize.w = 1.f / float(GraphHeight);

	SetUsePixelShader(pShader);
	SetUseTextureToShader(0, grhandle);
	SetPSConstF(22, GraphSize);
	SetPSConstF(23, Parameter);

	DrawPolygon2DToShader(Vertexes, 2);

	SetUseTextureToShader(0, -1);

	DrawLine(400, 0, 400, 480, 0x00ff00);
	DrawLine(0, 450, 640, 450, 0xff0000);
}

void WaveEffectRenderer::Apply() {
	int GraphWidth, GraphHeight;
	GetGraphSize(DX_SCREEN_BACK, &GraphWidth, &GraphHeight);

	GetDrawScreenGraph(0, 0, GraphWidth, GraphHeight, TempScreen);

	Position[0] = VGet(1.f, 1.f, 0.f);
	Position[1] = VGet(float(GraphWidth), 1.f, 0.f);
	Position[2] = VGet(1.f, float(GraphHeight), 0.f);
	Position[3] = VGet(float(GraphWidth), float(GraphHeight), 0.f);

	for (int i = 0; i < 6; i++)
		Vertexes[i].pos = Position[Index[i]];

	GraphSize.x = float(GraphWidth);
	GraphSize.y = float(GraphHeight);
	GraphSize.z = 1.f / float(GraphWidth);
	GraphSize.w = 1.f / float(GraphHeight);

	Parameter.x = Amplitude;
	Parameter.y = float(Flames / 60.f) * (2 * DX_PI_F);
	Parameter.z = Flequency;
	Parameter.w = (2 * DX_PI_F) / Lambda;

	SetUsePixelShader(pShader);
	SetUseTextureToShader(0, TempScreen);
	SetPSConstF(22, GraphSize);
	SetPSConstF(23, Parameter);

	DrawPolygon2DToShader(Vertexes, 2);

	SetUseTextureToShader(0, -1);
}