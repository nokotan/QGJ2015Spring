#include "BaseUnit.h"

// マップタイルの一辺の長さをここに書いてください。
// const Pixel MapTile::MapSize = 32;

/*
Map Map::DefaultMap = Map();

Map::Map() : Tiles(nullptr), Width(0), Height(0), IsTilesCopyed(false) {
	DefaultMap = *this;
}

Map::Map(int width, int height) : Map() {
	DefaultMap = *this;
	Create(width, height);
}

Map& Map::operator=(Map& right) {
	if (&right == this) {
		return *this;
	}

	Create(right.Width, right.Height, right.Tiles);
	return *this;
}

Map::~Map() {
	if (Tiles != nullptr) {
		if (!IsTilesCopyed) {
			delete[] Tiles[0];
		}

		delete[] Tiles;
	}
}

void Map::Create(int width, int height, MapTile::Type **source, bool forceclone) {
	Width = width;
	Height = height;
	IsTilesCopyed = (source != nullptr && !forceclone);

	if (Tiles != nullptr) {
		// デストラクタだけ呼び出す。
		Map::~Map();
	}

	Tiles = new MapTile::Type*[width];
	
	if (IsTilesCopyed) {
		for (int i = 0; i < width; i++) {
			Tiles[i] = source[i];
		}
	} else {
		MapTile::Type *newtiles = new MapTile::Type[width * height];

		for (int i = 0; i < width; i++) {
			Tiles[i] = newtiles + (i * height);
		}
	}
}

void Map::SetTile(MapTile::Type& tile, int x, int y) {
	if (x >= 0 && x < Width && y >= 0 && y < Height) {
		Tiles[x][y] = tile;
	} else {
		// Will throw.
	}
}

MapTile::Type &Map::GetTile(int x, int y) {
	if (x >= 0 && x < Width && y >= 0 && y < Height) {
		return Tiles[x][y];
	} else {
		return MapTile::Empty;
	}
}

DynamicArray<MapTile::Type> Map::operator[](int x) {
	return DynamicArray<MapTile::Type>(Tiles[x], Height);
}
*/