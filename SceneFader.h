#pragma once

// SceneFader.h ---------------------------------
// 画面や画像にさまざまなエフェクトをかけるクラス
// ----------------------------------------------

#include "DxLib.h"

// 画像の透過部分と非透過部分を指定した色に変換して描画するクラス。
class ImageMaskRenderer {
	int pShader;

	VECTOR Position[4];
	VERTEX2DSHADER Vertexes[6];
	int Index[6];

	// コピーの禁止
	ImageMaskRenderer(const ImageMaskRenderer&) = delete;
	ImageMaskRenderer &operator=(const ImageMaskRenderer&) = delete;
public:
	ImageMaskRenderer();

	// 初期化します。
	bool Initialize();
	// このオブジェクトが持つすべてのリソースを解放します。
	void Dispose();		

	// 非透過および透過部分を指定した色に置き換えて描画します。
	void DrawImage(int x, int y, int grHandle, COLOR_U8 backgroundcolor, COLOR_U8 forecolor);
	// 非透過および透過部分を指定した色に置き換えて描画します。
	void DrawExtendedImage(int x1, int y1, int x2, int y2, int GrHandle, COLOR_U8 backgroundcolor, COLOR_U8 forecolor);
};



// 画像の解像度を変更して描画するクラス。
class ImageMosaicRenderer {
	bool IsInitialized;

	int pShader;

	VECTOR Position[4];
	VERTEX2DSHADER Vertexes[6];
	int Index[6];
	FLOAT4 Dpi;

	// コピーの禁止
	ImageMosaicRenderer(const ImageMosaicRenderer&) = delete;
	ImageMosaicRenderer &operator=(const ImageMosaicRenderer&) = delete;
public:
	ImageMosaicRenderer();

	// 初期化します。
	bool Initialize();
	// このオブジェクトが持つすべてのリソースを解放します。
	void Dispose();		
	
	// 指定した解像度で描画します。
	void DrawImage(int x, int y, int dpix, int dpiy, int grHandle);
};



// 画面に透過部分と非透過部分を指定して描画するクラス。
class StencilBuffer {
	int Width, Height;
	int MaskGraph;

	ImageMaskRenderer MaskRenderer;

	// コピーの禁止
	StencilBuffer(const StencilBuffer&) = delete;
	StencilBuffer &operator= (const StencilBuffer&) = delete;
public:
	StencilBuffer();
	~StencilBuffer();

	int GetHandle() const {
		return MaskGraph;
	}

	// 初期化します。
	bool Initialize(int width, int height);
	// このオブジェクトが持つすべてのリソースを解放します。
	void Dispose();

	// 画面をクリアします。
	void Clear();
	// 現在のマスクの状態を適用します。
	void Apply() const;

	// マスク画面に四角形を追加します。
	void AddRectangle(int x1, int y1, int x2, int y2, unsigned int color = GetColor(0, 0, 0));
	// マスク画面に円を追加します。
	void AddCircle(int x, int y, int r, unsigned int color = GetColor(0, 0, 0));
	// マスク画面に画像を追加します。
	void AddImage(int x, int y, int grHandle, COLOR_U8 backgroundcolor = GetColorU8(0, 0, 0, 255), COLOR_U8 forecolor = GetColorU8(255, 255, 255, 255));
	// マスク画面に画像を追加します。
	void AddExtendedImage(int x1, int y1, int x2, int y2, int grHandle, COLOR_U8 backgroundcolor = GetColorU8(0, 0, 0, 255), COLOR_U8 forecolor = GetColorU8(255, 255, 255, 255));
	
	// 
	void AddString(int x, int y, const char text[], unsigned int color = GetColor(0, 0, 0));
};



// 画面の解像度を変更して描画するクラス。
class MosaicRenderer {
	int Width, Height;
	int MaskGraph;
	ImageMosaicRenderer Mosaic;

	// コピーの禁止
	MosaicRenderer(const MosaicRenderer&) = delete;
	MosaicRenderer &operator=(const MosaicRenderer&) = delete;
public:
	MosaicRenderer();

	// 初期化します。
	bool Initialize(int width, int height);
	// このオブジェクトが持つすべてのリソースを解放します。
	void Dispose();

	// 画面の解像度を指定して、適用します。
	void Apply(int dpix, int dpiy);
};



// 画像がはためいているように描画するクラス。
class WaveEffectRenderer {
	int pShader;
	int TempScreen;

	int Flames;
	// x:振幅, y:時刻*2*PI(), z:振動数, w:波長の逆数*2*PI()
	FLOAT4 Parameter;

	VECTOR Position[4];
	VERTEX2DSHADER Vertexes[6];
	int Index[6];
	FLOAT4 GraphSize;

	// コピーの禁止
	WaveEffectRenderer(const WaveEffectRenderer&) = delete;
	WaveEffectRenderer &operator=(const WaveEffectRenderer&) = delete;
public:
	// 振幅
	float Amplitude;
	// 周波数
	float Flequency;
	// 波長
	float Lambda;

	WaveEffectRenderer();

	// 初期化します。
	bool Initialize();
	// このオブジェクトが持つすべてのリソースを解放します。
	void Dispose();

	// エフェクトを更新します
	void Update();
	// 指定した座標にエフェクトを適用して描画します。
	void DrawImage(int x, int y, int grhandle);
	// エフェクトを適用します。
	void Apply();
};



// 画面が割れるようなエフェクトを描画するクラス。
class ShutterRenderer {
	int Width, Height;
	int MaskGraph;

	static const int TriangleMax = 256;

	VERTEX2D Vertex[TriangleMax * 3];
	int VertexNum;

	// 移動速度
	float DeltaY[TriangleMax];
	// 移動速度
	float DeltaX[TriangleMax];
	// 重心位置（回転の中心）
	VECTOR GPos[TriangleMax];
	// Cos値
	float DeltaCos[TriangleMax];
	// Sin値
	float DeltaSin[TriangleMax];

	// コピーの禁止
	ShutterRenderer(const ShutterRenderer&) = delete;
	ShutterRenderer &operator=(const ShutterRenderer&) = delete;

	void AddVertex(int index, VECTOR& v);
public:
	ShutterRenderer() = default;

	// 初期化します。
	bool Initialize(int width, int height);
	// リセット
	void Reset();
	// このオブジェクトが持つすべてのリソースを明示的に解放します。
	void Dispose();

	// エフェクトが終わったかどうか。
	bool EffectFinished() const;

	// エフェクトを更新します。
	void Update();
	// エフェクトを適用します。
	void Apply() const;
};





// 画面に波紋が広がっていくようなエフェクトを描画するクラス。
class WaveEffect2Renderer {
	int pShader;
	int TempScreen;
	int DeltaScreen;
	int Width, Height;

	int Flames;

	VECTOR Position[4];
	VERTEX2DSHADER Vertexes[6];
	int Index[6];

	VECTOR CenterPos;

	// コピーの禁止
	WaveEffect2Renderer(const WaveEffect2Renderer&) = delete;
	WaveEffect2Renderer &operator=(const WaveEffect2Renderer&) = delete;
public:
	WaveEffect2Renderer();

	// 光方向ベクトル
	VECTOR Norm;
	// 減衰距離
	float Decay;
	// 振幅
	float Amplitude;
	// 周波数
	float Flequency;
	// 波長
	float Lambda;

	// 中心座標を設定します。
	void SetCenterPos(float x, float y);
	// カウンタをリセットします。
	void Reset();

	// 初期化します。
	bool Initialize();
	// このオブジェクトが持つすべてのリソースを解放します。
	void Dispose();

	// エフェクトを更新します。
	void Update();
	// エフェクトを適用します。
	void Apply();
};