#include "GameResourseManager.h"
#include "DxLib.h"

#include <string>

// 画像の読み込みに必要なデータを格納します。
// データは、LoadDivGraph に使うものです。
struct GraphData {
	std::string FileName;
	int AllNum;
	int XNum, YNum;
	int XSize, YSize;

	GraphData(std::string filename, int allnum = 0, int xnum = 0, int ynum = 0, int xsize = 0, int ysize = 0) :
		FileName(filename), 
		AllNum(allnum),
		XNum(xnum), YNum(ynum),
		XSize(xsize), YSize(ysize) {
	}
};

static GraphData ImageFileNames[] {
	// ここにイメージファイルのパスを追加します。
	// GameImages の定義での各要素の順番と対応するように画像のパスを追加してください。
	// 例１）LoadDivGraph を使う場合
	// ※ LoadDivGraph 関数に渡す引数の順番で書いてください。
	//	{ "Player.png", 16, 2, 8, 64, 64 }
	// 例２）LoadGraph を使う場合
	//  "Enemy.png"
	"",
	"",
	""
};

static std::string MusicFileNames[] {
	// ここに音楽ファイルのパスを追加します。
	""
};



GameResourseManager::GameResourseManager() {
	grHandles = new int*[GameImages_Count] {};
	musHandles = new int[GameMusics_Count] {};
}

GameResourseManager::~GameResourseManager() {
	delete[] grHandles;
	delete[] musHandles;
}

int GameResourseManager::GetGameImageHandle(GameImages val) {
#ifdef _DEBUG
	if (val < 0 || val >= GameImages_Count) {
		DebugTrace("ArgumentOutOfRengeException: val must not be under zero or over %d!\n", GameImages_Count - 1);
		return DX_NONE_GRAPH;
	}
#endif // _DEBUG

	return grHandles[val][0];
}

int *GameResourseManager::GetGameImageHandleArray(GameImages val) {
#ifdef _DEBUG
	if (val < 0 || val >= GameImages_Count) {
		DebugTrace("ArgumentOutOfRengeException: val must not be under zero or over %d!\n", GameImages_Count - 1);
		return nullptr;
	}
#endif // _DEBUG

	return (grHandles[val]);
}

int GameResourseManager::GetGameImageHandleCount(GameImages val) {
#ifdef _DEBUG
	if (val < 0 || val >= GameImages_Count) {
		DebugTrace("ArgumentOutOfRengeException: val must not be under zero or over %d!\n", GameImages_Count - 1);
		return -1;
	}
#endif // _DEBUG

	return ImageFileNames[val].AllNum;
}

int GameResourseManager::GetGameMusicHandle(GameMusics val) {
#ifdef _DEBUG
	if (val < 0 || val >= GameMusics_Count) {
		DebugTrace("ArgumentOutOfRengeException: val must not be under zero or over %d!\n", GameMusics_Count - 1);
		return DX_NONE_GRAPH;
	}
#endif // _DEBUG

	return musHandles[val];
}

bool GameResourseManager::InitializeImages() {
#ifdef _DEBUG
	if (_countof(ImageFileNames) < GameImages_Count) {
		DebugTrace("NullReferencedException: ImageFileNames の要素数が足りません。（%d個必要です）\n", GameImages_Count);
		return true;
	}
#endif

	for (int i = 0; i < GameImages_Count; i++) {
		if (ImageFileNames[i].AllNum > 1) {
			grHandles[i] = new int[ImageFileNames[i].AllNum] {};

			LoadDivGraph(ImageFileNames[i].FileName.c_str(), ImageFileNames[i].AllNum, ImageFileNames[i].XNum, ImageFileNames[i].YNum,
						 ImageFileNames[i].XSize, ImageFileNames[i].YSize, grHandles[i]);
		} else {
			grHandles[i] = new int[1] {};

			grHandles[i][0] = LoadGraph(ImageFileNames[i].FileName.c_str());
		}

		AssertTrace(grHandles[i][0] != -1, "FileNotFoundException: 画像ファイル \"%s\" の初期化に失敗しました。\n", ImageFileNames[i].FileName.c_str());
	}
	
	return false;
}

void GameResourseManager::DisposeImages() {
	for (int i = 0; i < GameImages_Count; i++) {
		if (grHandles[i] == nullptr)
			continue;

		for (int j = 0; j < ImageFileNames[i].AllNum; j++) {
			DeleteGraph(grHandles[i][j]);
		}

		delete[] grHandles[i];
	}
}

bool GameResourseManager::InitializeMusics() {
#ifdef _DEBUG
	if (_countof(MusicFileNames) < GameMusics_Count) {
		DebugTrace("NullReferencedException: MusicFileNames の要素数が足りません。（%d個必要です）\n", GameMusics_Count);
		return true;
	}
#endif

	for (int i = 0; i < GameMusics_Count; i++) {
		musHandles[i] = LoadSoundMem(MusicFileNames[i].c_str());
		AssertTrace(musHandles[i] != -1, "FileNotFoundException: 音楽ファイル \"%s\" の初期化に失敗しました。\n", MusicFileNames[i].c_str());
	}

	return false;
}

void GameResourseManager::DisposeMusics() {
	for (int i = 0; i < GameMusics_Count; i++)
		DeleteSoundMem(musHandles[i]);
}





int ScreenCapture(HWND hwnd) {
	// BMP情報作成
	BITMAPINFO bmpInfo = {};
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = 1366;
	bmpInfo.bmiHeader.biHeight = 768;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 24;

	// DIBを作成しメモリブロックを貰う
	unsigned char *p = 0;
	HBITMAP hBitMap = CreateDIBSection(0, &bmpInfo, DIB_RGB_COLORS, (void**)&p, 0, 0);

	// スクリーンのデバイスコンテキストを取得
	HDC hDC = GetWindowDC(hwnd);
	HDC hDCNew = CreateCompatibleDC(hDC);

	HBITMAP defBMP = (HBITMAP)SelectObject(hDCNew, (HGDIOBJ)hBitMap);
	BitBlt(hDCNew, 0, 0, 1366, 768, hDC, 0, 0, SRCCOPY);
	SelectObject(hDCNew, (HGDIOBJ)defBMP);

	DeleteDC(hDCNew);
	ReleaseDC(hwnd, hDC);

	int grHandle = CreateGraphFromBmp(&bmpInfo, (void*)p);
	DeleteObject((HGDIOBJ)hBitMap);

	return grHandle;
}