#include "GameResourseManager.h"
#include "DxLib.h"
#include <unordered_map>
#include <string>

class ResourceManager {
	std::unordered_map<std::string, int> Handles;

	// コンストラクタは通常
	ResourceManager() = default;
public:
	// コピーコンストラクタは使えない。
	ResourceManager(const ResourceManager&) = delete;
	// 代入演算子も使えない。
	ResourceManager &operator=(ResourceManager&) = delete;

	static ResourceManager &GetInstance() {
		static ResourceManager Instance;
		return Instance;
	}

	int RegisterHandle(const std::string& Key, int Handle) {
		if (Handle != -1) {
			return (Handles[Key] = Handle);
		} else {
			return -1;
		}
	}

	int GetHandle(const std::string& Key) const {
		if (Handles.find(Key) != Handles.end()) {
			return Handles.at(Key);
		} else {
			return -1;
		}
	}
};


int LoadGraphToAsset(const char FileName[]) {
	// ファイルのフルパスを保存するバッファ
	char FullPath[MAX_PATH];

	// フルパスを取得
	DxLib::ConvertFullPath(FileName, FullPath);

	// グラフィックハンドルを読み込み、それを登録する。
	return ResourceManager::GetInstance().RegisterHandle(FullPath, LoadGraph(FileName));
}

int LoadSoundMemToAsset(const char FileName[]) {
	// ファイルのフルパスを保存するバッファ
	char FullPath[MAX_PATH];

	// フルパスを取得
	DxLib::ConvertFullPath(FileName, FullPath);

	// グラフィックハンドルを読み込み、それを登録する。
	return ResourceManager::GetInstance().RegisterHandle(FullPath, LoadSoundMem(FileName));
}

int GetHandleToAsset(const char FileName[]) {
	// ファイルのフルパスを保存するバッファ
	char FullPath[MAX_PATH];

	// フルパスを取得
	DxLib::ConvertFullPath(FileName, FullPath);

	return ResourceManager::GetInstance().GetHandle(FullPath);
}