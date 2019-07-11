#include "SceneFader.h"
#include <new>

ImageMaskRenderer::ImageMaskRenderer() {
	new(Index) int[6] { 0, 1, 3, 0, 3, 2 };
	
	VECTOR TexturePosition[4] =
		{ VGet(0.f, 0.f, 0.f), VGet(1.f, 0.f, 0.f), VGet(0.f, 1.f, 0.f), VGet(1.f, 1.f, 0.f) };

	for (int i = 0; i < 6; i++) {
		Vertexes[i].rhw = 1.f;
		Vertexes[i].u = TexturePosition[Index[i]].x;
		Vertexes[i].v = TexturePosition[Index[i]].y;
	}
}

bool ImageMaskRenderer::Initialize() {
	return (pShader = LoadPixelShader("ShaderFiles/StencilBufferPixelShader.pso")) != -1;
}

void ImageMaskRenderer::Dispose() {
	DeleteShader(pShader);
}

void ImageMaskRenderer::DrawImage(int x, int y, int grHandle, COLOR_U8 backgroundcolor, COLOR_U8 forecolor) {
	int GraphWidth, GraphHeight;
	GetGraphSize(grHandle, &GraphWidth, &GraphHeight);

	DrawExtendedImage(x, y, x + GraphWidth, y + GraphHeight, grHandle, backgroundcolor, forecolor);
}

void ImageMaskRenderer::DrawExtendedImage(int x1, int y1, int x2, int y2, int grHandle, COLOR_U8 backgroundcolor, COLOR_U8 forecolor) {
	Position[0] = VGet(float(x1), float(y1), 0.f);
	Position[1] = VGet(float(x2), float(y1), 0.f);
	Position[2] = VGet(float(x1), float(y2), 0.f);
	Position[3] = VGet(float(x2), float(y2), 0.f);
	
	for (int i = 0; i < 6; i++) {
		Vertexes[i].pos = Position[Index[i]];
		Vertexes[i].dif = forecolor;
		Vertexes[i].spc = backgroundcolor;
	}

	SetUsePixelShader(pShader);
	SetUseTextureToShader(0, grHandle);
	
	DrawPolygon2DToShader(Vertexes, 2);

	SetUseTextureToShader(0, -1);
}