#pragma once

#include "BaseUnit.h"
#include "MemberCheck.h"

/// <summary>向きを表す列挙型を定義します。</summary>
namespace Direction {
	/// <summary>向きを表す列挙型を定義します。</summary>
	enum Direction {
		/// <summary>どの方向でもない</summary>
		None = 0,
		/// <summary>左向き</summary>
		Left = 1,
		/// <summary>右向き</summary>
		Right = 2,
		/// <summary>上向き</summary>
		Up = 4,
		/// <summary>下向き</summary>
		Down = 8,
		/// <summary>左向きと右向き</summary>
		LeftAndRight = 3,
		/// <summary>上向きと下向き</summary>
		UpAndDown = 12
	};
}

namespace Collider2D {
	// あたり判定を行うための設定 ---------------------

	// using *** = +++; という宣言で、
	// +++ という型が *** という名前でも使えるようになります。
	// 使用例:
	// using MyType = int;
	// MyType PlayerX;
	// // これは次のように書くのと同じ
	// // int PlayerX;

	/// <summary>
	///	あたり判定の対象となる構造体。メンバ変数として、
	///	現在位置: x, y、速度: dx, dy, サイズ: width, height を
	/// 持っている必要があります。
	/// </summary>
	using ColliderType = struct Player;
	/// <summary>マップタイルとして使う型</summary>
	using MapTileType = int;
	/// <summary>マップタイルの１つの幅</summary>
	const int MapTileSize = 32;
	/// <summary>マップの横方向のタイルの数</summary>
	const int MapWidth = 20;
	/// <summary>マップの横方向のタイルの数</summary>
	const int MapHeight = 15;

	// ------------------------------------------------

	template <class ColliderType>
	/// <summary>渡されたオブジェクトの座標値更新とあたり判定を行います。</summary>
	/// <param name="ColliderObject">あたり判定を行う構造体のインスタンス</param>
	int CollisionCheck(ColliderType& Object, std::vector<IMapTileLayor<int>*>& Layors) {
		// ぶつかった方向
		int CollidedDirection = Direction::None;
		// 床の速度
		Pixel FloorSpeed = 0;
		
		// y軸方向のあたり判定 -------------------------------------------------------------------------------------------------

		// 新しいy座標の値
		Pixel NewY = Object.y + Object.dy;
		// 新しいy軸方向の速度
		Pixel NewDy = Object.dy;

		for (IMapTileLayor<int>* pLayor : Layors) {
			if (pLayor == static_cast<void*>(&Object)) {
				continue;
			}

			IMapTileLayor<int>& Layor = *pLayor;

			// あたり判定を行われる構造体の存在領域を表す矩形
			Rectangle_t RectY{ static_cast<Pixel>(Object.x + Layor.GetDeltaX()), static_cast<Pixel>(NewY), static_cast<Pixel>(Object.width), static_cast<Pixel>(Object.height) };

			// あたり判定の領域のx軸方向の開始値
			int FirstX = static_cast<int>(floor(RectY.X - Layor.GetX()) / static_cast<double>(Layor.GetTileSize()));
			// あたり判定の領域のx軸方向の終了値
			int LastX = static_cast<int>((RectY.X + RectY.Width - 1 - Layor.GetX()) / static_cast<double>(Layor.GetTileSize()));
			// あたり判定の領域のy軸方向の開始値
			int FirstY = static_cast<int>(floor(RectY.Y - Layor.GetY()) / static_cast<double>(Layor.GetTileSize()));
			// あたり判定の領域のy軸方向の終了値
			int LastY = static_cast<int>((RectY.Y + RectY.Height - 1 - Layor.GetY()) / static_cast<double>(Layor.GetTileSize()));

			for (int i = FirstX; i <= LastX; i++) {
				for (int j = FirstY; j <= LastY; j++) {
					MapTileType& Tile = Layor.GetTile(i, j);
					// マップタイルの存在領域を表す矩形
					Rectangle_t TileRect{ i * Layor.GetTileSize() + Layor.GetX(), j * Layor.GetTileSize() + Layor.GetY(), Layor.GetTileSize(), Layor.GetTileSize() };

					if (RectY.IntersectWith(TileRect) && Tile != -1) {
						// y軸方向の相対速度
						Pixel RelationalSpeed = Object.dy - Layor.GetDeltaY();

						// 相対速度が下向きならば
						if (RelationalSpeed > 0) {
							// 新しいy座標の値と速度を補正
							NewY = TileRect.Y - RectY.Height;
							NewDy = Layor.GetDeltaY();
							FloorSpeed = Layor.GetDeltaX();
							CollidedDirection |= Direction::Down;

							// ここに下からブロックにぶつかったときの処理を追加

						}
						else if (RelationalSpeed < 0) {
							// 新しいy座標の値と速度を補正
							NewY = TileRect.Y + TileRect.Height;
							NewDy = Layor.GetDeltaY();
							CollidedDirection |= Direction::Up;

							// ここに下からブロックにぶつかったときの処理を追加

						}
					}
				}
			}
		}

		Object.y = NewY;
		Object.dy = NewDy;

		// ---------------------------------------------------------------------------------------------------------------------

		// x軸方向のあたり判定 -------------------------------------------------------------------------------------------------

		Pixel NewX = Object.x + Object.dx + FloorSpeed;
		Pixel NewDx = Object.dx + FloorSpeed;

		for (IMapTileLayor<int>* pLayor : Layors) {
			if (pLayor == static_cast<void*>(&Object)) {
				continue;
			}

			IMapTileLayor<int>& Layor = *pLayor;
			Rectangle_t RectX{ static_cast<Pixel>(NewX), static_cast<Pixel>(Object.y + Layor.GetDeltaY()), static_cast<Pixel>(Object.width), static_cast<Pixel>(Object.height) };

			int FirstX = static_cast<int>(floor((RectX.X - Layor.GetX()) / static_cast<double>(Layor.GetTileSize())));
			int LastX = static_cast<int>((RectX.X + RectX.Width - 1 - Layor.GetX()) / static_cast<double>(Layor.GetTileSize()));
			int FirstY = static_cast<int>(floor((RectX.Y - Layor.GetY()) / static_cast<double>(Layor.GetTileSize())));
			int LastY = static_cast<int>((RectX.Y + RectX.Height - 1 - Layor.GetY()) / static_cast<double>(Layor.GetTileSize()));

			for (int i = FirstX; i <= LastX; i++) {
				for (int j = FirstY; j <= LastY; j++) {
					MapTileType& Tile = Layor.GetTile(i, j);
					// マップタイルの存在領域を表す矩形
					Rectangle_t TileRect{ i * Layor.GetTileSize() + Layor.GetX(), j * Layor.GetTileSize() + Layor.GetY() + Layor.GetDeltaY(), Layor.GetTileSize(), Layor.GetTileSize() };

					if (RectX.IntersectWith(TileRect) && Tile != -1) {
						// x軸方向の相対速度
						Pixel RelationalSpeed = Object.dx + FloorSpeed - Layor.GetDeltaX();

						if (RelationalSpeed > 0) {
							NewX = TileRect.X - RectX.Width;
							NewDx = Layor.GetDeltaX();
							CollidedDirection |= Direction::Right;

							// ここに右からブロックにぶつかったときの処理を追加

						}
						else if (RelationalSpeed < 0) {
							NewX = TileRect.X + TileRect.Width;
							NewDx = Layor.GetDeltaX();
							CollidedDirection |= Direction::Left;

							// ここに左からブロックにぶつかったときの処理を追加

						}
						else {
							CollidedDirection |= Direction::LeftAndRight;
						}
					}
				}
			}
		}

		Object.x = NewX;
		Object.dx = NewDx;

		// ---------------------------------------------------------------------------------------------------------------------

		return CollidedDirection;
	}

	template <class ColliderType>
	/// <summary>渡されたオブジェクトの座標値更新とあたり判定を行います。</summary>
	/// <param name="ColliderObject">あたり判定を行う構造体のインスタンス</param>
	/// <param name="MapTiles">マップタイルとして使う２次元配列</param>
	int CollisionCheck(ColliderType& ColliderObject, MapTileType (&MapTiles)[MapWidth][MapHeight]) {
		static_assert(CheckMember<ColliderType>::HasXY, "ColliderObject must have these member variables! : x, y");
		static_assert(CheckMember<ColliderType>::HasDxDy, "ColliderObject must have these member variables! : dx, dy");
		static_assert(CheckMember<ColliderType>::HasWidthHeight, "ColliderObject must have these member variables! : width, height");

		return CollisionCheck(ColliderObject, TemporaryMap<MapTileType, MapWidth, MapHeight>(MapTiles));
	};
}