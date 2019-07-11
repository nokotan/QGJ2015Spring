#include "SceneFader.h"

#include <new>
#include <math.h>

WaveEffect2Renderer::WaveEffect2Renderer() :
	Norm(VGet(1.f, 0.f, 0.f)),
	Decay(600.f),
	Lambda(16.f),
	Amplitude(5.f),
	Flequency(8.f),
	CenterPos(VGet(.5f, .5f, .0f)) {

	new(Index) int[6] { 0, 1, 3, 0, 3, 2 };

	VECTOR TexturePosition[4] =
	{ VGet(0.f, 0.f, 0.f), VGet(1.f, 0.f, 0.f), VGet(0.f, 1.f, 0.f), VGet(1.f, 1.f, 0.f) };

	for (int i = 0; i < 6; i++) {
		Vertexes[i].rhw = 1.f;
		Vertexes[i].u = TexturePosition[Index[i]].x;
		Vertexes[i].v = TexturePosition[Index[i]].y;
	}

	Flames = 0;
}

void WaveEffect2Renderer::SetCenterPos(float x, float y) {
	CenterPos.x = x / Width;
	CenterPos.y = y / Height;
}

void WaveEffect2Renderer::Reset() {
	Flames = 0;
}

bool WaveEffect2Renderer::Initialize() {
	GetGraphSize(DX_SCREEN_BACK, &Width, &Height);
	TempScreen = MakeScreen(Width, Height);
	DeltaScreen = MakeScreen(Width, Height);
	return (pShader = LoadPixelShader("ShaderFiles/WaveEffect2PixelShader.pso"));
}

void WaveEffect2Renderer::Dispose() {
	DeleteShader(pShader);
	DeleteGraph(TempScreen);
	DeleteGraph(DeltaScreen);
}

void WaveEffect2Renderer::Update() {
	Flames++;

	//SetDrawScreen(DeltaScreen);
	//
	//float CenterX = Width / 2.f;
	//float CenterY = Height / 2.f;
	//
	//for (int x = 0; x < Width; x++) {
	//	for (int y = 0; y < Height; y++) {
	//		float DistanceX = x - CenterX, DistanceY = y - CenterY;
	//		float Distance = sqrt(DistanceX * DistanceX + DistanceY * DistanceY);
	//		float LocalFlame = (Flames - Distance) * .1f;
	//
	//		if (LocalFlame < 0) LocalFlame = 0;
	//
	//		float Delta = sin(LocalFlame);
	//		float DeltaX = Delta * DistanceX / Distance; 
	//		float DeltaY = Delta * DistanceY / Distance;
	//
	//		int Red = DeltaX * 10.f + 127;
	//		int Green = DeltaY * 10.f + 127;
	//
	//		DrawPixel(x, y, GetColor(Red, Green, 0));
	//	}
	//}
	//
	//SetDrawScreen(DX_SCREEN_BACK);
}

void WaveEffect2Renderer::Apply() {
	GetDrawScreenGraph(0, 0, Width, Height, TempScreen);

	Position[0] = VGet(0.f, 0.f, 0.f);
	Position[1] = VGet(float(Width - 1.f), 0.f, 0.f);
	Position[2] = VGet(0.f, float(Height - 1.f), 0.f);
	Position[3] = VGet(float(Width - 1.f), float(Height - 1.f), 0.f);

	for (int i = 0; i < 6; i++)
		Vertexes[i].pos = Position[Index[i]];

	// x:Œo‰ßŽžŠÔ, y:Žü”g”*2*PI, z:”g’·‚Ì‹t”, w:”g”
	FLOAT4 WaveParam;
	// x,y,z:Œõ•ûŒüƒxƒNƒgƒ‹, w:‰A‰e‚Ì‹­‚³
	FLOAT4 NormParam;
	// x:Œ¸Š‹——£, y:Œ¸Š‹——£‚Ì‹t”, z,w:U•
	FLOAT4 DecayParam;
	// x,y:’†SÀ•W, z,w:‰æ–ÊƒTƒCƒY
	FLOAT4 GraphSize;

	WaveParam.x = Flames / 60.f;
	WaveParam.y = Flequency * 2 * DX_PI_F;
	WaveParam.z = 2 * DX_PI_F / Lambda;
	WaveParam.w = 2 * DX_PI_F;

	NormParam.x = Norm.x;
	NormParam.y = Norm.y;
	NormParam.z = Norm.z;
	NormParam.w = .3f;
	
	DecayParam.x = Decay;
	DecayParam.y = 1 / Decay;
	DecayParam.z = Amplitude / Width;
	DecayParam.w = Amplitude / Height;

	GraphSize.x = CenterPos.x;
	GraphSize.y = CenterPos.y;
	GraphSize.z = Width;
	GraphSize.w = Height;

	SetUsePixelShader(pShader);
	SetUseTextureToShader(0, TempScreen);
	SetUseTextureToShader(1, DeltaScreen);
	SetPSConstF(22, WaveParam);
	SetPSConstF(23, NormParam);
	SetPSConstF(24, DecayParam);
	SetPSConstF(25, GraphSize);

	DrawPolygon2DToShader(Vertexes, 2);

	SetUseTextureToShader(0, -1);
	SetUseTextureToShader(1, -1);
}