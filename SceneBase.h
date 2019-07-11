#pragma once

/// @file SceneBase.h
/// SceneBase.h --------------------------------
/// ��ʁi�V�[�P���X�j�J�ڃV�X�e����񋟂��܂��B
/// �ŏI�X�V:
///			2016/08/18
/// --------------------------------------------
/// @remark
///	�g�p��:
/// SceneBase.h
/// @code
/// #pragma once
///	#define SceneTypes(macro)	\
///		macro(Title)			\
///		macro(Game)				\
///		macro(Ending)
///	
///	// �ȉ��ҏW�̕K�v���Ȃ��̂ŏȗ�
/// @endcode
///	TitleScene.cpp
///	@code
///	#include "SceneBase.h"
/// #include "DxLib.h"
///
/// class TitleScene : public Scene {
///		// �����Ƀ^�C�g����ʂŕK�v�ƂȂ�ϐ���ǉ����Ă��������B
///	public:
///		bool Initialize() override;
///		void Update() override;
///		void Draw() override;
///	};
///
///	CreateScene(TitleScene);
///
/// bool TitleScene::Initialize() {
///		return false;
///	}
///
///	void TitleScene::Update() {
///		// [Z]�L�[�������ꂽ���ʕύX
///		if (CheckHitKey(KEY_INPUT_Z)) {
///			OnSceneChange(ST_GameScene);
///		}
/// }
///
///	void TitleScene::Draw() {
///		DrawString(3, 3, "This is Title Scene!", GetColor(255, 255, 255));
///		DrawString(3, 50, "Press [Z] Key!!", GetColor(255, 255, 255));
///	}
///	@endcode
///	GameScene.cpp
///	@code
///	#include "SceneBase.h"
/// #include "DxLib.h"
///
/// class GameScene : public Scene {
///		// �����ɃQ�[����ʂŕK�v�ƂȂ�ϐ���ǉ����Ă��������B
///		int Counter;
///	public:
///		bool Initialize() override;
///		void Update() override;
///		void Draw() override;
///	};
///
///	CreateScene(GameScene);
///
/// bool GameScene::Initialize() {
///		Counter = 0;
///		return false;
///	}
///
///	void GameScene::Update() {
///		Counter++;
///
///		// �T�b���������ʕύX
///		if (Counter >= 5 * 60) {
///			OnSceneChange(ST_EndingScene);
///		}
/// }
///
///	void GameScene::Draw() {
///		DrawString(3, 3, "This is Game Scene!", GetColor(255, 255, 255));
///		DrawFormatString(3, 50, GetColor(255, 255, 255), "�؂�ւ��܂� %d �b", 5 - Counter / 60);
///	}
///	@endcode
///	EndingScene.cpp
///	@code
///	#include "SceneBase.h"
/// #include "DxLib.h"
///
/// class EndingScene : public Scene {
///		// �����Ƀ��U���g��ʂŕK�v�ƂȂ�ϐ���ǉ����Ă��������B
///		int Counter;
///	public:
///		bool Initialize() override;
///		void Update() override;
///		void Draw() override;
///	};
///
///	CreateScene(EndingScene);
///
/// bool EndingScene::Initialize() {
///		Counter = 0;
///		return false;
///	}
///
///	void EndingScene::Update() {
///		Counter++;
///
///		// �T�b���������ʕύX
///		if (Counter >= 5 * 60) {
///			OnSceneChange(ST_TitleScene);
///		}
/// }
///
///	void EndingScene::Draw() {
///		DrawString(3, 3, "This is Ending Scene!", GetColor(255, 255, 255));
///		DrawFormatString(3, 50, GetColor(255, 255, 255), "�؂�ւ��܂� %d �b", 5 - Counter / 60);
///	}
///	@endcode
///	Main.cpp
/// @code
/// #include "SceneBase.h"
/// #include "DxLib.h"
///
///	int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
///		ChangeWindowMode(TRUE);
///
///		if (DxLib_Init() == -1) {
///			return 0;
///		};
///
///		SetDrawScreen(DX_SCREEN_BACK);
///		
///		SceneManager MySceneManager;
///		MySceneManager.OnSceneChange(ST_Title);
///
///		while (ProcessMessage() != -1 && ClearDrawScreen() != -1) {
///			MySceneManager.Update();
///			MySceneManager.Draw();
///		}
///
///		DxLib_End();
///		return 0;
///	}
///	@endcode


/// <summary>�V�[������p�}�N��</summary>
/// <remarks>�V�[����ǉ�����Ƃ��͂����ɂ��̖��O�𑫂��Ă��������B</remarks>
#define SceneTypes(macro)	\
	macro(Opening)			\
	macro(Level)			\
	macro(Ending)			\
	macro(GameOver)			\
	macro(Shutter)			\
	macro(EffectTest)		\
	macro(WaveTest)			\
	macro(Reflection)

/// <summary>�w�肵���V�[���N���X�����̉����邽�߂̊֐����쐬���܂��B</summary>
/// <param name="name">���̉����邽�߂̊֐����쐬����V�[���N���X�̖��O</param>
/// <example>
/// �g�p��:
/// ���ӁI
/// ���̃T���v���ł́A�֐����g�����߂̍ŏ����̕��������Љ�Ă��܂��B
/// �t�@�C�� "SceneBase.h" �Ɏ������T���v�����Q�l�ɂ��Ȃ���ǂ�ł��������B
/// TitleScene.cpp
/// <code>
/// class TitleScene : public Scene {
///		// �����Ƀ^�C�g����ʂŕK�v�ƂȂ�ϐ���ǉ����Ă��������B
///	public:
///		bool Initialize() override;
///		void Update() override;
///		void Draw() override;
///	};
///
/// CreateScene(TitleScene);
/// </code>
/// </example>
#define CreateScene(name)					\
	template<>								\
	Scene* Scene::CreateInstance<name>() {	\
		return new name();					\
	} 

/// <summary>SceneType �񋓌^�̊e�v�f��錾���܂��B</summary>
#define DeclearSceneType(name) ST_##name,

/// <summary>��ʁi�V�[���j�̎�ށB�e�V�[���ɑ΂��āAST_(�N���X��) �Ƃ����`���̒l������܂��B</summary>
enum SceneType {
	/// <summary>�ǂ̃V�[���ł��Ȃ����Ƃ�\���l</summary>
	ST_NULL,

	SceneTypes(DeclearSceneType)
};

#undef DeclearSceneType



#pragma region Scene

#include <functional>

/// <summary>��ʁi�V�[���j�̊��N���X</summary>
class Scene {
public:
	/// <summary>�V���� Scene �I�u�W�F�N�g���쐬���܂��B</summary>
	Scene();
	virtual ~Scene();

	/// <summary>���������܂��B</summary>
	virtual bool Initialize();
	/// <summary>�X�V���܂��B</summary>
	virtual void Update() = 0;
	/// <summary>�`�悵�܂��B</summary>
	virtual void Draw() = 0;

	template <class SceneClass, class ...Args>
	/// <summary>�V�[���̃C���X�^���X�쐬</summary>
	static Scene *CreateInstance(Args...);

#pragma region Event
private:
	std::function<void(SceneType)> scenechange;
protected:
	/// <summary>��ʑJ�ڎ��ɌĂяo���֐����Ăяo���܂��B</summary>
	void OnSceneChange(SceneType);
public:
	/// <summary>��ʑJ�ڎ��ɌĂяo���֐���o�^���܂��B</summary>
	void AddHandler(std::function<void(SceneType)>);
	/// <summary>��ʑJ�ڎ��ɌĂяo���֐����폜���܂��B</summary>
	void RemoveHandler();
#pragma endregion
};

#pragma endregion



/// <summary>��ʂ��Ǘ����܂��B</summary>
class SceneManager {
	Scene *CurrentScene;
	SceneType CurrentSceneType;
	SceneType NextScene;

	void SceneChange();

	SceneManager(SceneManager&);
	SceneManager &operator=(SceneManager&);
public:
	/// <summary>�V���� SceneManager �I�u�W�F�N�g�����������܂��B</summary>
	SceneManager();

	/// <summary>�V���� SceneManager �I�u�W�F�N�g�����������܂��B</summary>
	/// <param name="nextScene">�ŏ��̃V�[����\�� SceneType �񋓑̂̒l</param>
	/// <example>
	/// �g�p��:
	/// ���ӁI
	/// ���̃T���v���ł́A�֐����g�����߂̍ŏ����̕��������Љ�Ă��܂��B
	/// �t�@�C�� "SceneBase.h" �Ɏ������T���v�����Q�l�ɂ��Ȃ���ǂ�ł��������B
	/// <code>
	/// // �ŏ��̃V�[�����^�C�g���V�[���ɐݒ�
	///	SceneManager MySceneManager { ST_Title };
	/// </code>
	/// <example>
	SceneManager(SceneType nextScene);
	~SceneManager();

	/// <summary>�V�[�����X�V���܂��B</summary>
	void Update();
	/// <summary>�V�[����`�悵�܂��B</summary>
	void Draw() const;

	/// <summary>�V�[����ύX���܂��B</summary>
	/// <param name="nextScene">���̃V�[����\�� SceneType �񋓑̂̒l</param>
	/// <example>
	/// �g�p��:
	/// ���ӁI
	/// ���̃T���v���ł́A�֐����g�����߂̍ŏ����̕��������Љ�Ă��܂��B
	/// �t�@�C�� "SceneBase.h" �Ɏ������T���v�����Q�l�ɂ��Ȃ���ǂ�ł��������B
	/// <code>
	///	SceneManager MySceneManager;
	/// // �^�C�g���V�[���ɐ؂�ւ�
	///	MySceneManager.OnSceneChange(ST_Title);
	/// </code>
	/// <example>
	void OnSceneChange(SceneType nextScene);
};