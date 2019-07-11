#include "SceneFader.h"

StencilBuffer::StencilBuffer() :
	Width(0), Height(0), MaskGraph(DX_NONE_GRAPH) {
}

StencilBuffer::~StencilBuffer() {
}

bool StencilBuffer::Initialize(int width, int height) {
	Width = width;
	Height = height;

	if (MaskGraph != DX_NONE_GRAPH)
		DeleteGraph(MaskGraph);

	MaskGraph = MakeScreen(width, height);
	MaskRenderer.Initialize();

	Clear();
	return (MaskGraph != DX_NONE_GRAPH);
}

void StencilBuffer::Dispose() {
	MaskRenderer.Dispose();
	DeleteGraph(MaskGraph);
}

void StencilBuffer::Clear() {
	AddRectangle(0, 0, Width, Height, GetColor(255, 255, 255));
}

void StencilBuffer::Apply() const {
	int DefBlendMode, DefBlendParam;
	GetDrawBlendMode(&DefBlendMode, &DefBlendParam);
	SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
	DrawGraph(0, 0, MaskGraph, TRUE);
	SetDrawBlendMode(DefBlendMode, DefBlendParam);
}

void StencilBuffer::AddRectangle(int x1, int y1, int x2, int y2, unsigned int color) {
	int DefScreen = GetDrawScreen();
	SetDrawScreen(MaskGraph);
	DrawBox(x1, y1, x2, y2, color, TRUE);
	SetDrawScreen(DefScreen);
}

void StencilBuffer::AddCircle(int x, int y, int r, unsigned int color) {
	int DefScreen = GetDrawScreen();
	SetDrawScreen(MaskGraph);
	DrawCircle(x, y, r, color);
	SetDrawScreen(DefScreen);
}

void StencilBuffer::AddString(int x, int y, const char text[], unsigned int color) {
	int DefScreen = GetDrawScreen();
	SetDrawScreen(MaskGraph);
	DrawString(x, y, text, color);
	SetDrawScreen(DefScreen);
}

void StencilBuffer::AddImage(int x, int y, int grHandle, COLOR_U8 backgroundcolor, COLOR_U8 forecolor) {
	int DefScreen = GetDrawScreen();
	SetDrawScreen(MaskGraph);
	MaskRenderer.DrawImage(x, y, grHandle, backgroundcolor, forecolor);
	SetDrawScreen(DefScreen);
}

void StencilBuffer::AddExtendedImage(int x1, int y1, int x2, int y2, int grHandle, COLOR_U8 backgroundcolor, COLOR_U8 forecolor) {
	int DefScreen = GetDrawScreen();
	SetDrawScreen(MaskGraph);
	MaskRenderer.DrawExtendedImage(x1, y1, x2, y2, grHandle, backgroundcolor, forecolor);
	SetDrawScreen(DefScreen);
}