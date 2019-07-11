#pragma once

#include "BaseUnit.h"
#include "MemberCheck.h"

/// <summary>������\���񋓌^���`���܂��B</summary>
namespace Direction {
	/// <summary>������\���񋓌^���`���܂��B</summary>
	enum Direction {
		/// <summary>�ǂ̕����ł��Ȃ�</summary>
		None = 0,
		/// <summary>������</summary>
		Left = 1,
		/// <summary>�E����</summary>
		Right = 2,
		/// <summary>�����</summary>
		Up = 4,
		/// <summary>������</summary>
		Down = 8,
		/// <summary>�������ƉE����</summary>
		LeftAndRight = 3,
		/// <summary>������Ɖ�����</summary>
		UpAndDown = 12
	};
}

namespace Collider2D {
	// �����蔻����s�����߂̐ݒ� ---------------------

	// using *** = +++; �Ƃ����錾�ŁA
	// +++ �Ƃ����^�� *** �Ƃ������O�ł��g����悤�ɂȂ�܂��B
	// �g�p��:
	// using MyType = int;
	// MyType PlayerX;
	// // ����͎��̂悤�ɏ����̂Ɠ���
	// // int PlayerX;

	/// <summary>
	///	�����蔻��̑ΏۂƂȂ�\���́B�����o�ϐ��Ƃ��āA
	///	���݈ʒu: x, y�A���x: dx, dy, �T�C�Y: width, height ��
	/// �����Ă���K�v������܂��B
	/// </summary>
	using ColliderType = struct Player;
	/// <summary>�}�b�v�^�C���Ƃ��Ďg���^</summary>
	using MapTileType = int;
	/// <summary>�}�b�v�^�C���̂P�̕�</summary>
	const int MapTileSize = 32;
	/// <summary>�}�b�v�̉������̃^�C���̐�</summary>
	const int MapWidth = 20;
	/// <summary>�}�b�v�̉������̃^�C���̐�</summary>
	const int MapHeight = 15;

	// ------------------------------------------------

	template <class ColliderType>
	/// <summary>�n���ꂽ�I�u�W�F�N�g�̍��W�l�X�V�Ƃ����蔻����s���܂��B</summary>
	/// <param name="ColliderObject">�����蔻����s���\���̂̃C���X�^���X</param>
	int CollisionCheck(ColliderType& Object, std::vector<IMapTileLayor<int>*>& Layors) {
		// �Ԃ���������
		int CollidedDirection = Direction::None;
		// ���̑��x
		Pixel FloorSpeed = 0;
		
		// y�������̂����蔻�� -------------------------------------------------------------------------------------------------

		// �V����y���W�̒l
		Pixel NewY = Object.y + Object.dy;
		// �V����y�������̑��x
		Pixel NewDy = Object.dy;

		for (IMapTileLayor<int>* pLayor : Layors) {
			if (pLayor == static_cast<void*>(&Object)) {
				continue;
			}

			IMapTileLayor<int>& Layor = *pLayor;

			// �����蔻����s����\���̂̑��ݗ̈��\����`
			Rectangle_t RectY{ static_cast<Pixel>(Object.x + Layor.GetDeltaX()), static_cast<Pixel>(NewY), static_cast<Pixel>(Object.width), static_cast<Pixel>(Object.height) };

			// �����蔻��̗̈��x�������̊J�n�l
			int FirstX = static_cast<int>(floor(RectY.X - Layor.GetX()) / static_cast<double>(Layor.GetTileSize()));
			// �����蔻��̗̈��x�������̏I���l
			int LastX = static_cast<int>((RectY.X + RectY.Width - 1 - Layor.GetX()) / static_cast<double>(Layor.GetTileSize()));
			// �����蔻��̗̈��y�������̊J�n�l
			int FirstY = static_cast<int>(floor(RectY.Y - Layor.GetY()) / static_cast<double>(Layor.GetTileSize()));
			// �����蔻��̗̈��y�������̏I���l
			int LastY = static_cast<int>((RectY.Y + RectY.Height - 1 - Layor.GetY()) / static_cast<double>(Layor.GetTileSize()));

			for (int i = FirstX; i <= LastX; i++) {
				for (int j = FirstY; j <= LastY; j++) {
					MapTileType& Tile = Layor.GetTile(i, j);
					// �}�b�v�^�C���̑��ݗ̈��\����`
					Rectangle_t TileRect{ i * Layor.GetTileSize() + Layor.GetX(), j * Layor.GetTileSize() + Layor.GetY(), Layor.GetTileSize(), Layor.GetTileSize() };

					if (RectY.IntersectWith(TileRect) && Tile != -1) {
						// y�������̑��Α��x
						Pixel RelationalSpeed = Object.dy - Layor.GetDeltaY();

						// ���Α��x���������Ȃ��
						if (RelationalSpeed > 0) {
							// �V����y���W�̒l�Ƒ��x��␳
							NewY = TileRect.Y - RectY.Height;
							NewDy = Layor.GetDeltaY();
							FloorSpeed = Layor.GetDeltaX();
							CollidedDirection |= Direction::Down;

							// �����ɉ�����u���b�N�ɂԂ������Ƃ��̏�����ǉ�

						}
						else if (RelationalSpeed < 0) {
							// �V����y���W�̒l�Ƒ��x��␳
							NewY = TileRect.Y + TileRect.Height;
							NewDy = Layor.GetDeltaY();
							CollidedDirection |= Direction::Up;

							// �����ɉ�����u���b�N�ɂԂ������Ƃ��̏�����ǉ�

						}
					}
				}
			}
		}

		Object.y = NewY;
		Object.dy = NewDy;

		// ---------------------------------------------------------------------------------------------------------------------

		// x�������̂����蔻�� -------------------------------------------------------------------------------------------------

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
					// �}�b�v�^�C���̑��ݗ̈��\����`
					Rectangle_t TileRect{ i * Layor.GetTileSize() + Layor.GetX(), j * Layor.GetTileSize() + Layor.GetY() + Layor.GetDeltaY(), Layor.GetTileSize(), Layor.GetTileSize() };

					if (RectX.IntersectWith(TileRect) && Tile != -1) {
						// x�������̑��Α��x
						Pixel RelationalSpeed = Object.dx + FloorSpeed - Layor.GetDeltaX();

						if (RelationalSpeed > 0) {
							NewX = TileRect.X - RectX.Width;
							NewDx = Layor.GetDeltaX();
							CollidedDirection |= Direction::Right;

							// �����ɉE����u���b�N�ɂԂ������Ƃ��̏�����ǉ�

						}
						else if (RelationalSpeed < 0) {
							NewX = TileRect.X + TileRect.Width;
							NewDx = Layor.GetDeltaX();
							CollidedDirection |= Direction::Left;

							// �����ɍ�����u���b�N�ɂԂ������Ƃ��̏�����ǉ�

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
	/// <summary>�n���ꂽ�I�u�W�F�N�g�̍��W�l�X�V�Ƃ����蔻����s���܂��B</summary>
	/// <param name="ColliderObject">�����蔻����s���\���̂̃C���X�^���X</param>
	/// <param name="MapTiles">�}�b�v�^�C���Ƃ��Ďg���Q�����z��</param>
	int CollisionCheck(ColliderType& ColliderObject, MapTileType (&MapTiles)[MapWidth][MapHeight]) {
		static_assert(CheckMember<ColliderType>::HasXY, "ColliderObject must have these member variables! : x, y");
		static_assert(CheckMember<ColliderType>::HasDxDy, "ColliderObject must have these member variables! : dx, dy");
		static_assert(CheckMember<ColliderType>::HasWidthHeight, "ColliderObject must have these member variables! : width, height");

		return CollisionCheck(ColliderObject, TemporaryMap<MapTileType, MapWidth, MapHeight>(MapTiles));
	};
}