#include "GameResourseManager.h"
#include "DxLib.h"
#include <unordered_map>
#include <string>

class ResourceManager {
	std::unordered_map<std::string, int> Handles;

	// �R���X�g���N�^�͒ʏ�
	ResourceManager() = default;
public:
	// �R�s�[�R���X�g���N�^�͎g���Ȃ��B
	ResourceManager(const ResourceManager&) = delete;
	// ������Z�q���g���Ȃ��B
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
	// �t�@�C���̃t���p�X��ۑ�����o�b�t�@
	char FullPath[MAX_PATH];

	// �t���p�X���擾
	DxLib::ConvertFullPath(FileName, FullPath);

	// �O���t�B�b�N�n���h����ǂݍ��݁A�����o�^����B
	return ResourceManager::GetInstance().RegisterHandle(FullPath, LoadGraph(FileName));
}

int LoadSoundMemToAsset(const char FileName[]) {
	// �t�@�C���̃t���p�X��ۑ�����o�b�t�@
	char FullPath[MAX_PATH];

	// �t���p�X���擾
	DxLib::ConvertFullPath(FileName, FullPath);

	// �O���t�B�b�N�n���h����ǂݍ��݁A�����o�^����B
	return ResourceManager::GetInstance().RegisterHandle(FullPath, LoadSoundMem(FileName));
}

int GetHandleToAsset(const char FileName[]) {
	// �t�@�C���̃t���p�X��ۑ�����o�b�t�@
	char FullPath[MAX_PATH];

	// �t���p�X���擾
	DxLib::ConvertFullPath(FileName, FullPath);

	return ResourceManager::GetInstance().GetHandle(FullPath);
}