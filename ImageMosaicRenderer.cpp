#include "SceneFader.h"
#include "DebugHelper.h"
#include <new>

#define UseShader

ImageMosaicRenderer::ImageMosaicRenderer() :
	pShader(-1), IsInitialized(false) {
	new(Index) int[6] { 0, 1, 3, 0, 3, 2 };

	VECTOR TexturePosition[4] =
	{ VGet(0.f, 0.f, 0.f), VGet(1.f, 0.f, 0.f), VGet(0.f, 1.f, 0.f), VGet(1.f, 1.f, 0.f) };
	
	for (int i = 0; i < 6; i++) {
		Vertexes[i].rhw = 1.f;
		Vertexes[i].u = TexturePosition[Index[i]].x;
		Vertexes[i].v = TexturePosition[Index[i]].y;
	}
}

bool ImageMosaicRenderer::Initialize() {
	if (!IsInitialized) {
		pShader = LoadPixelShader("ShaderFiles/MosaicPixelShader.pso");
		IsInitialized = true;
	}

	return pShader != -1;
}

void ImageMosaicRenderer::Dispose() {
	DeleteShader(pShader);
}

void ImageMosaicRenderer::DrawImage(int x, int y, int dpix, int dpiy, int grHandle) {
	AssertTrace(!IsInitialized, "NotInitializedException : ‰Šú‰»‚³‚ê‚é‘O‚Éƒƒ“ƒoŠÖ”‚ªŒÄ‚Î‚ê‚Ü‚µ‚½B\n");

	int width, height;
	GetGraphSize(grHandle, &width, &height);

#ifdef UseShader 
	Position[0] = VGet(float(x), float(y), 0.f);
	Position[1] = VGet(float(x + width - 1), float(y), 0.f);
	Position[2] = VGet(float(x), float(y + height - 1), 0.f);
	Position[3] = VGet(float(x + width - 1), float(y + height - 1), 0.f);

	for (int i = 0; i < 6; i++)
		Vertexes[i].pos = Position[Index[i]];

	Dpi.x = (float)dpix;
	Dpi.y = (float)dpiy;
	Dpi.z = 1.f / dpix;
	Dpi.w = 1.f / dpiy;

	SetUsePixelShader(pShader);
	SetUseTextureToShader(0, grHandle);
	SetPSConstF(22, Dpi);

	DrawPolygon2DToShader(Vertexes, 2);

	SetUseTextureToShader(0, -1);
#else
	int TempGraph = MakeScreen(dpix, dpiy);
	
	SetDrawScreen(TempGraph);
	DrawExtendGraph(0, 0, dpix, dpiy, grHandle, FALSE);
	SetDrawScreen(DX_SCREEN_BACK);
	DrawExtendGraph(0, 0, width, height, TempGraph, FALSE);
	DeleteGraph(TempGraph);
#endif
}

#undef UseShader