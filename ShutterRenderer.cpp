#include "SceneFader.h"

#include <math.h>

void ShutterRenderer::AddVertex(int index, VECTOR& v) {
	Vertex[index].pos = v;
	Vertex[index].u = (v.x + .5f) / Width;
	Vertex[index].v = (v.y + .5f) / Height;
	Vertex[index].dif = GetColorU8(255, 255, 255, 255);
	Vertex[index].rhw = 1.f;
}

// コンパイラがうるさいので、警告抑制
#pragma warning(push)
#pragma warning(disable:4244)

bool ShutterRenderer::Initialize(int width, int height) {
	Width = width;
	Height = height;

	MaskGraph = MakeScreen(width, height);
	Reset();

	return 0;
}

void ShutterRenderer::Reset() {
	// 行程
	int PrevTripLength = 0, TripLength = 0;
	// 行程の最大値
	const int TripLengthMax = (Width + Height) * 2;
	// 中心座標
	auto&& Center = VGet(Width / 2, Height / 2, 0);

	// 行程から座標を取得します。
	auto GetPosition = [&](int s) { 
		if (s < Width)
			return VGet(s, 0, 0);
		else if (s < Width + Height)
			return VGet(Width, s - Width, 0);
		else if (s < Width * 2 + Height)
			return VGet(Width * 2 + Height - s, Height, 0);
		else if (s < (Width + Height) * 2)
			return VGet(0, (Width + Height) * 2 - s, 0);
		else
			return VGet(0, 0, 0);
	};

	// 行程から辺の番号を取得します。
	auto GetNo = [&](int s) {
		if (s < Width)
			return 0;
		else if (s < Width + Height)
			return 1;
		else if (s < Width * 2 + Height)
			return 2;
		else
			return 3;
	};

	// 辺の番号から角の座標を取得します。
	auto GetCorner = [&](int s) {
		if (s == 0) {
			return VGet(0, 0, 0);
		} else if (s == 1) {
			return VGet(Width, 0, 0);
		} else if (s == 2) {
			return VGet(Width, Height, 0);
		} else {
			return VGet(0, Height, 0);
		}
	};

	int Count = 0;

	while (true) {
		// 行程の移動距離
		const int DivLength = GetRand(100) + Height / 3 - 100;
		// 三角形をいくつに分割するか行程から適当に決定
		const int DivNum = DivLength / 20;

		PrevTripLength = TripLength;
		TripLength += DivLength;

		auto Position1 = GetPosition(PrevTripLength);
		auto Position2 = GetPosition(TripLength);

		for (int i = 0; i < DivNum; i++) {
			// 0.5 ～ 0.8 の乱数を生成（最後だけ除く）
			float s = i != (DivNum - 1) ? GetRand(30) / 100.f + .5f : 1.f;
			// 内分点を取得
			auto Position3 = VGet(Position2.x * (1 - s) + Center.x * s, Position2.y * (1 - s) + Center.y * s, 0);

			// 物理挙動のパラメータを設定（初速と重心位置、角速度）
			DeltaY[Count / 3] = (GetRand(10) - 5) * i / 3.f; // (Position1.y - Height) / 80 + GetRand(6) - 3;
			// ここも、適当な乱数を用いて
			DeltaX[Count / 3] = (GetRand(10) - 5) * i / 3.f;

			GPos[Count / 3].x = (Position1.x + Position2.x + Position3.x) / 3;
			GPos[Count / 3].y = (Position1.y + Position2.y + Position3.y) / 3;

			float DeltaTheta = GetRand(20) / 200.f - .05f;
			DeltaCos[Count / 3] = cos(DeltaTheta);
			DeltaSin[Count / 3] = sin(DeltaTheta);

			// 行程が角を超えるとき
			if (i == 0 && GetNo(PrevTripLength) != GetNo(TripLength)) {
				auto Corner = GetCorner(GetNo(TripLength));

				AddVertex(Count++, Position1);
				AddVertex(Count++, Position2);
				AddVertex(Count++, Corner);

				DeltaY[Count / 3] = DeltaY[Count / 3 - 1]; 
				GPos[Count / 3] = GPos[Count / 3 - 1];
				DeltaCos[Count / 3] = DeltaCos[Count / 3 - 1];
				DeltaSin[Count / 3] = DeltaSin[Count / 3 - 1];
			}

			// 作成した三角形の頂点を反時計回りに格納
			if (i % 2 == 0) {
				AddVertex(Count++, Position2);
				AddVertex(Count++, Position1);
				AddVertex(Count++, Position3);
			} else {
				AddVertex(Count++, Position1);
				AddVertex(Count++, Position2);
				AddVertex(Count++, Position3);
			}

			Position2 = Position1;
			Position1 = Position3;
		}

		if (TripLength >= TripLengthMax)	break;
	}

	VertexNum = Count;
}

#pragma warning(pop)

void ShutterRenderer::Dispose() {
	DeleteGraph(MaskGraph);
}

bool ShutterRenderer::EffectFinished() const {
	for (int i = 0; i < VertexNum; i++) {
		if (Vertex[i].pos.y <= Height) {
			return false;
		}
	}

	return true;
}

void ShutterRenderer::Update() {
	for (int i = 0; i < VertexNum / 3; i++) {
		DeltaY[i] += .15f;
		
		if (true || DeltaY[i] > 0) {
			GPos[i].y += DeltaY[i];
			GPos[i].x += DeltaX[i];

			for (int j = 0; j < 3; j++) {
				int index = i * 3 + j;
				Vertex[index].pos.y += DeltaY[i];
				Vertex[index].pos.x += DeltaX[i];

				float revx = Vertex[index].pos.x - GPos[i].x;
				float revy = -(Vertex[index].pos.y - GPos[i].y);

				revx = DeltaCos[i] * revx - DeltaSin[i] * revy;
				revy = DeltaSin[i] * revx + DeltaCos[i] * revy;

				Vertex[index].pos.x = revx + GPos[i].x;
				Vertex[index].pos.y = -revy + GPos[i].y;
			}
		}
	}
}

void ShutterRenderer::Apply() const {
	GetDrawScreenGraph(0, 0, Width, Height, MaskGraph);

	// 割れた後の背景の処理はここにそうぞ
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, Width, Height, 0, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawPolygon2D(Vertex, VertexNum / 3, MaskGraph, TRUE);

	// どのように分割されているか気になるときは、ここをコメントアウト
	//for (int i = 0; i < VertexNum; i += 3) {
	//	DrawTriangle(Vertex[i].pos.x, Vertex[i].pos.y, Vertex[i + 1].pos.x, Vertex[i + 1].pos.y, Vertex[i + 2].pos.x, Vertex[i + 2].pos.y, GetColor(255, 0, 0), FALSE);
	//}
}