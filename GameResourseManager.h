#pragma once

/**
* @file GameResourseManager.h
* GameResourseManager.h ------------------------
* �Q�[�����Ŏg�p���鉹�y��摜�Ȃǂ��Ǘ����܂��B
* ----------------------------------------------
*/

#include "DebugHelper.h"

/// <summary>�摜�����ʂ���萔</summary>
/// <remarks>�K�v�ɉ����ėv�f��ǉ����Ă��������B</remarks>
enumdef(GameImages,
	Player,
	Enemy,
	Neutral
)

/// <summary>���y�����ʂ���萔</summary>
/// <remarks>�K�v�ɉ����ėv�f��ǉ����Ă��������B</remarks>
enumdef(GameMusics,

)

/// <summary>�ȈՃA�N�Z�X�p�}�N��</summary>
#define GameResourseMgr GameResourseManager::Instance()

/// <summary>
/// �Q�[�����Ŏg�p���鉹�y��摜�Ȃǂ��Ǘ����܂��B
/// [Singleton �N���X] 
/// �C���X�^���X�������ł��Ȃ��̂ŁAGameResourseManager::Instance() �֐� �܂��� GameResourseMgr �}�N�����g���ăC���X�^���X�ɃA�N�Z�X���Ă��������B
/// </summary>
/// <example>
/// �g�p��:
/// GameResourseManager.h
/// <code>
/// #pragma once
///
/// #include "DebugHelper.h"
///
/// enumdef(GameImages,
///		PlayerImage,
///		EnemyImage,
///		ItemImage
/// )
/// 
/// enumdef(GameMusics,
///		PlayerJumpSound,
///		DamageSound,
///		MainBGM
///	)
/// 
///	// �ȉ��ҏW�̕K�v���Ȃ��̂ŏȗ�
/// </code>
///	GameResourseManager.cpp
/// <code>
/// #include "GameResourseManager.h"
/// #include "DxLib.h"
///
/// #include &lt;string&gt;
///
/// �摜�̓ǂݍ��݂ɕK�v�ȃf�[�^���i�[���܂��B
/// �f�[�^�́ALoadDivGraph �Ɏg�����̂ł��B
/// struct GraphData {
///		// ��`�͏ȗ�	
/// };
///
/// // �v���C���[�A�G�A�A�C�e���̉摜�̖��O�����ꂼ��A"Player.png", "Enemy.png", "Item.png"
/// // �ł��鎞�̏������ł��B���ۂɎg���t�@�C�����ɉ����ď��������Ă��������B
/// static GraphData ImageFileNames[] {
///		"Player.png",	// PlayerImage
///		"Enemy.png",	// EnemyImage
///		"Item.png"		// ItemImage
/// };
///
///	static std::string MusicFileNames[] {
///		"Jump.ogg",		// PlayerJumpSound
///		"Damage.ogg",	// DamageSound
///		"MainBGM.ogg"	// MainBGM
///	};
///
///	// �ȉ��ҏW�̕K�v���Ȃ��̂ŏȗ�
/// </code>
/// �g�p����
/// <code>
///	#include "GameResourseManager.h"
/// #include "DxLib.h"
///
/// int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
///		ChangeWindowMode(TRUE);
///		
///		if (DxLib_Init() == -1) {
///			return 0;
///		}
///
///		SetDrawScreen(DX_SCREEN_BACK);	
///		GameResourseMgr.InitializeImages();
///
///		PlaySoundMem(GameResourseMgr.GetGameMusicHandle(MainBGM), DX_PLAYTYPE_BACK);
///
///		// ���C�����[�v
///		while (ProcessMessage() != -1 &amp;&amp; ClearDrawScreen() != -1) {
///			DrawGraph(100, 100, GameResourseMgr.GameImageHandleArray(PlayerImage)[0], TRUE);
///			DrawGraph(100, 100, GameResourseMgr.GameImageHandle(EnemyImage), TRUE);
///			
///			ScreenFlip();
///		}
///
///		DxLib_End();
///		return 0;
///	}
/// </code>
/// </example>
class GameResourseManager {
	int **grHandles;
	int *musHandles;

	// �C���X�^���X��������уR�s�[�֎~
	GameResourseManager();
	GameResourseManager(const GameResourseManager&);
	GameResourseManager &operator=(const GameResourseManager&);
public:
	~GameResourseManager();

	/// <summary>GameResourseManager �C���X�^���X���擾���܂��B</summary>
	/// <returns>GameResourseManager �C���X�^���X�ւ̎Q��</returns>
	static GameResourseManager &Instance() {
		static GameResourseManager instance;
		return instance;
	}
	
	/// <summary>�摜�����������܂��B</summary>
	/// <returns>�������ɐ�������� false, �����łȂ���� true</returns>
	bool InitializeImages();
	/// <summary>�摜�𖾎��I�ɉ�����܂��B</summary>
	void DisposeImages();
	/// <summary>���y�����������܂��B</summary>
	/// <returns>�������ɐ�������� false, �����łȂ���� true</returns>
	bool InitializeMusics();
	/// <summary>���y�𖾎��I�ɉ�����܂��B</summary>
	void DisposeMusics();

	/// <summary>�w�肵���摜�̃n���h�����擾���܂��B</summary>
	int GetGameImageHandle(GameImages);
	/// <summary>�w�肵���摜�̃n���h�����擾���܂��B</summary>
	int *GetGameImageHandleArray(GameImages);
	/// <summary>�w�肵���摜�̐����擾���܂��B</summary>
	int GetGameImageHandleCount(GameImages);
	/// <summary>�w�肵�����y�̃n���h�����擾���܂��B</summary>
	int GetGameMusicHandle(GameMusics);
};

// �w�肵���E�B���h�E�̕`����e���L���v�`�����܂��B
int ScreenCapture(struct HWND__* hwnd = nullptr);

/// <summary>�V�����摜��ǂݍ��݁A�A�Z�b�g�}�l�[�W���ɂ��̃n���h����o�^���܂��B</summary>
/// <param name="FileName">�V�����ǂݍ��މ摜�t�@�C���̃p�X</param>
/// <returns>�o�^�ɐ�������΂��̃O���t�B�b�N�n���h��, �����łȂ���� -1</returns>
/// <remarks>���łɓo�^�����g���Ă����ꍇ�͉��������A�֘A�t����ꂽ�O���t�B�b�N�n���h����Ԃ��܂��B</remarks>
int LoadGraphToAsset(const char FileName[]);

/// <summary>�V����������ǂݍ��݁A�A�Z�b�g�}�l�[�W���ɂ��̃n���h����o�^���܂��B</summary>
/// <param name="FileName">�V�����ǂݍ��މ����t�@�C���̃p�X</param>
/// <returns>�o�^�ɐ�������΂��̉����n���h��, �����łȂ���� -1</returns>
/// <remarks>���łɓo�^�����g���Ă����ꍇ�͉��������A�֘A�t����ꂽ�����n���h����Ԃ��܂��B</remarks>
int LoadSoundMemToAsset(const char FileName[]);

/// <summary>�A�Z�b�g�}�l�[�W������w�肵���o�^���Ɋ֘A�t�����Ă���摜�≹���̃n���h�������o���܂��B</summary>
/// <param name="FileName">���łɓǂݍ���ł���摜�≹���t�@�C���̃p�X</param>
/// <returns>�n���h���̎擾�ɐ�������΁A���̃n���h���B�����łȂ���΁A-1</returns>
int GetHandleToAsset(const char FileName[]);