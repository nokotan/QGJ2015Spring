#include "SceneFader.h"

#define UseShader

MosaicRenderer::MosaicRenderer() {
}

bool MosaicRenderer::Initialize(int width, int height) {
	Width = width;
	Height = height;

	MaskGraph = MakeScreen(width, height);
	Mosaic.Initialize();

	return MaskGraph != DX_NONE_GRAPH;
}

void MosaicRenderer::Dispose() {
	Mosaic.Dispose();
	DeleteGraph(MaskGraph);
}

void MosaicRenderer::Apply(int dpix, int dpiy) {
	GetDrawScreenGraph(0, 0, Width, Height, MaskGraph);
	Mosaic.DrawImage(0, 0, dpix, dpiy, MaskGraph);
}