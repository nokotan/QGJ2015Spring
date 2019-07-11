#pragma once

// SceneFader.h ---------------------------------
// ��ʂ�摜�ɂ��܂��܂ȃG�t�F�N�g��������N���X
// ----------------------------------------------

#include "DxLib.h"

// �摜�̓��ߕ����Ɣ񓧉ߕ������w�肵���F�ɕϊ����ĕ`�悷��N���X�B
class ImageMaskRenderer {
	int pShader;

	VECTOR Position[4];
	VERTEX2DSHADER Vertexes[6];
	int Index[6];

	// �R�s�[�̋֎~
	ImageMaskRenderer(const ImageMaskRenderer&) = delete;
	ImageMaskRenderer &operator=(const ImageMaskRenderer&) = delete;
public:
	ImageMaskRenderer();

	// ���������܂��B
	bool Initialize();
	// ���̃I�u�W�F�N�g�������ׂẴ��\�[�X��������܂��B
	void Dispose();		

	// �񓧉߂���ѓ��ߕ������w�肵���F�ɒu�������ĕ`�悵�܂��B
	void DrawImage(int x, int y, int grHandle, COLOR_U8 backgroundcolor, COLOR_U8 forecolor);
	// �񓧉߂���ѓ��ߕ������w�肵���F�ɒu�������ĕ`�悵�܂��B
	void DrawExtendedImage(int x1, int y1, int x2, int y2, int GrHandle, COLOR_U8 backgroundcolor, COLOR_U8 forecolor);
};



// �摜�̉𑜓x��ύX���ĕ`�悷��N���X�B
class ImageMosaicRenderer {
	bool IsInitialized;

	int pShader;

	VECTOR Position[4];
	VERTEX2DSHADER Vertexes[6];
	int Index[6];
	FLOAT4 Dpi;

	// �R�s�[�̋֎~
	ImageMosaicRenderer(const ImageMosaicRenderer&) = delete;
	ImageMosaicRenderer &operator=(const ImageMosaicRenderer&) = delete;
public:
	ImageMosaicRenderer();

	// ���������܂��B
	bool Initialize();
	// ���̃I�u�W�F�N�g�������ׂẴ��\�[�X��������܂��B
	void Dispose();		
	
	// �w�肵���𑜓x�ŕ`�悵�܂��B
	void DrawImage(int x, int y, int dpix, int dpiy, int grHandle);
};



// ��ʂɓ��ߕ����Ɣ񓧉ߕ������w�肵�ĕ`�悷��N���X�B
class StencilBuffer {
	int Width, Height;
	int MaskGraph;

	ImageMaskRenderer MaskRenderer;

	// �R�s�[�̋֎~
	StencilBuffer(const StencilBuffer&) = delete;
	StencilBuffer &operator= (const StencilBuffer&) = delete;
public:
	StencilBuffer();
	~StencilBuffer();

	int GetHandle() const {
		return MaskGraph;
	}

	// ���������܂��B
	bool Initialize(int width, int height);
	// ���̃I�u�W�F�N�g�������ׂẴ��\�[�X��������܂��B
	void Dispose();

	// ��ʂ��N���A���܂��B
	void Clear();
	// ���݂̃}�X�N�̏�Ԃ�K�p���܂��B
	void Apply() const;

	// �}�X�N��ʂɎl�p�`��ǉ����܂��B
	void AddRectangle(int x1, int y1, int x2, int y2, unsigned int color = GetColor(0, 0, 0));
	// �}�X�N��ʂɉ~��ǉ����܂��B
	void AddCircle(int x, int y, int r, unsigned int color = GetColor(0, 0, 0));
	// �}�X�N��ʂɉ摜��ǉ����܂��B
	void AddImage(int x, int y, int grHandle, COLOR_U8 backgroundcolor = GetColorU8(0, 0, 0, 255), COLOR_U8 forecolor = GetColorU8(255, 255, 255, 255));
	// �}�X�N��ʂɉ摜��ǉ����܂��B
	void AddExtendedImage(int x1, int y1, int x2, int y2, int grHandle, COLOR_U8 backgroundcolor = GetColorU8(0, 0, 0, 255), COLOR_U8 forecolor = GetColorU8(255, 255, 255, 255));
	
	// 
	void AddString(int x, int y, const char text[], unsigned int color = GetColor(0, 0, 0));
};



// ��ʂ̉𑜓x��ύX���ĕ`�悷��N���X�B
class MosaicRenderer {
	int Width, Height;
	int MaskGraph;
	ImageMosaicRenderer Mosaic;

	// �R�s�[�̋֎~
	MosaicRenderer(const MosaicRenderer&) = delete;
	MosaicRenderer &operator=(const MosaicRenderer&) = delete;
public:
	MosaicRenderer();

	// ���������܂��B
	bool Initialize(int width, int height);
	// ���̃I�u�W�F�N�g�������ׂẴ��\�[�X��������܂��B
	void Dispose();

	// ��ʂ̉𑜓x���w�肵�āA�K�p���܂��B
	void Apply(int dpix, int dpiy);
};



// �摜���͂��߂��Ă���悤�ɕ`�悷��N���X�B
class WaveEffectRenderer {
	int pShader;
	int TempScreen;

	int Flames;
	// x:�U��, y:����*2*PI(), z:�U����, w:�g���̋t��*2*PI()
	FLOAT4 Parameter;

	VECTOR Position[4];
	VERTEX2DSHADER Vertexes[6];
	int Index[6];
	FLOAT4 GraphSize;

	// �R�s�[�̋֎~
	WaveEffectRenderer(const WaveEffectRenderer&) = delete;
	WaveEffectRenderer &operator=(const WaveEffectRenderer&) = delete;
public:
	// �U��
	float Amplitude;
	// ���g��
	float Flequency;
	// �g��
	float Lambda;

	WaveEffectRenderer();

	// ���������܂��B
	bool Initialize();
	// ���̃I�u�W�F�N�g�������ׂẴ��\�[�X��������܂��B
	void Dispose();

	// �G�t�F�N�g���X�V���܂�
	void Update();
	// �w�肵�����W�ɃG�t�F�N�g��K�p���ĕ`�悵�܂��B
	void DrawImage(int x, int y, int grhandle);
	// �G�t�F�N�g��K�p���܂��B
	void Apply();
};



// ��ʂ������悤�ȃG�t�F�N�g��`�悷��N���X�B
class ShutterRenderer {
	int Width, Height;
	int MaskGraph;

	static const int TriangleMax = 256;

	VERTEX2D Vertex[TriangleMax * 3];
	int VertexNum;

	// �ړ����x
	float DeltaY[TriangleMax];
	// �ړ����x
	float DeltaX[TriangleMax];
	// �d�S�ʒu�i��]�̒��S�j
	VECTOR GPos[TriangleMax];
	// Cos�l
	float DeltaCos[TriangleMax];
	// Sin�l
	float DeltaSin[TriangleMax];

	// �R�s�[�̋֎~
	ShutterRenderer(const ShutterRenderer&) = delete;
	ShutterRenderer &operator=(const ShutterRenderer&) = delete;

	void AddVertex(int index, VECTOR& v);
public:
	ShutterRenderer() = default;

	// ���������܂��B
	bool Initialize(int width, int height);
	// ���Z�b�g
	void Reset();
	// ���̃I�u�W�F�N�g�������ׂẴ��\�[�X�𖾎��I�ɉ�����܂��B
	void Dispose();

	// �G�t�F�N�g���I��������ǂ����B
	bool EffectFinished() const;

	// �G�t�F�N�g���X�V���܂��B
	void Update();
	// �G�t�F�N�g��K�p���܂��B
	void Apply() const;
};





// ��ʂɔg�䂪�L�����Ă����悤�ȃG�t�F�N�g��`�悷��N���X�B
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

	// �R�s�[�̋֎~
	WaveEffect2Renderer(const WaveEffect2Renderer&) = delete;
	WaveEffect2Renderer &operator=(const WaveEffect2Renderer&) = delete;
public:
	WaveEffect2Renderer();

	// �������x�N�g��
	VECTOR Norm;
	// ��������
	float Decay;
	// �U��
	float Amplitude;
	// ���g��
	float Flequency;
	// �g��
	float Lambda;

	// ���S���W��ݒ肵�܂��B
	void SetCenterPos(float x, float y);
	// �J�E���^�����Z�b�g���܂��B
	void Reset();

	// ���������܂��B
	bool Initialize();
	// ���̃I�u�W�F�N�g�������ׂẴ��\�[�X��������܂��B
	void Dispose();

	// �G�t�F�N�g���X�V���܂��B
	void Update();
	// �G�t�F�N�g��K�p���܂��B
	void Apply();
};