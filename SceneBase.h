#pragma once

/// @file SceneBase.h
/// SceneBase.h --------------------------------
/// 画面（シーケンス）遷移システムを提供します。
/// 最終更新:
///			2016/08/18
/// --------------------------------------------
/// @remark
///	使用例:
/// SceneBase.h
/// @code
/// #pragma once
///	#define SceneTypes(macro)	\
///		macro(Title)			\
///		macro(Game)				\
///		macro(Ending)
///	
///	// 以下編集の必要がないので省略
/// @endcode
///	TitleScene.cpp
///	@code
///	#include "SceneBase.h"
/// #include "DxLib.h"
///
/// class TitleScene : public Scene {
///		// ここにタイトル画面で必要となる変数を追加してください。
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
///		// [Z]キーが押されたら画面変更
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
///		// ここにゲーム画面で必要となる変数を追加してください。
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
///		// ５秒たったら画面変更
///		if (Counter >= 5 * 60) {
///			OnSceneChange(ST_EndingScene);
///		}
/// }
///
///	void GameScene::Draw() {
///		DrawString(3, 3, "This is Game Scene!", GetColor(255, 255, 255));
///		DrawFormatString(3, 50, GetColor(255, 255, 255), "切り替わるまで %d 秒", 5 - Counter / 60);
///	}
///	@endcode
///	EndingScene.cpp
///	@code
///	#include "SceneBase.h"
/// #include "DxLib.h"
///
/// class EndingScene : public Scene {
///		// ここにリザルト画面で必要となる変数を追加してください。
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
///		// ５秒たったら画面変更
///		if (Counter >= 5 * 60) {
///			OnSceneChange(ST_TitleScene);
///		}
/// }
///
///	void EndingScene::Draw() {
///		DrawString(3, 3, "This is Ending Scene!", GetColor(255, 255, 255));
///		DrawFormatString(3, 50, GetColor(255, 255, 255), "切り替わるまで %d 秒", 5 - Counter / 60);
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


/// <summary>シーン制御用マクロ</summary>
/// <remarks>シーンを追加するときはここにその名前を足してください。</remarks>
#define SceneTypes(macro)	\
	macro(Opening)			\
	macro(Level)			\
	macro(Ending)			\
	macro(GameOver)			\
	macro(Shutter)			\
	macro(EffectTest)		\
	macro(WaveTest)			\
	macro(Reflection)

/// <summary>指定したシーンクラスを実体化するための関数を作成します。</summary>
/// <param name="name">実体化するための関数を作成するシーンクラスの名前</param>
/// <example>
/// 使用例:
/// 注意！
/// このサンプルでは、関数を使うための最小限の部分だけ紹介しています。
/// ファイル "SceneBase.h" に示したサンプルも参考にしながら読んでください。
/// TitleScene.cpp
/// <code>
/// class TitleScene : public Scene {
///		// ここにタイトル画面で必要となる変数を追加してください。
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

/// <summary>SceneType 列挙型の各要素を宣言します。</summary>
#define DeclearSceneType(name) ST_##name,

/// <summary>画面（シーン）の種類。各シーンに対して、ST_(クラス名) という形式の値があります。</summary>
enum SceneType {
	/// <summary>どのシーンでもないことを表す値</summary>
	ST_NULL,

	SceneTypes(DeclearSceneType)
};

#undef DeclearSceneType



#pragma region Scene

#include <functional>

/// <summary>画面（シーン）の基底クラス</summary>
class Scene {
public:
	/// <summary>新しい Scene オブジェクトを作成します。</summary>
	Scene();
	virtual ~Scene();

	/// <summary>初期化します。</summary>
	virtual bool Initialize();
	/// <summary>更新します。</summary>
	virtual void Update() = 0;
	/// <summary>描画します。</summary>
	virtual void Draw() = 0;

	template <class SceneClass, class ...Args>
	/// <summary>シーンのインスタンス作成</summary>
	static Scene *CreateInstance(Args...);

#pragma region Event
private:
	std::function<void(SceneType)> scenechange;
protected:
	/// <summary>画面遷移時に呼び出す関数を呼び出します。</summary>
	void OnSceneChange(SceneType);
public:
	/// <summary>画面遷移時に呼び出す関数を登録します。</summary>
	void AddHandler(std::function<void(SceneType)>);
	/// <summary>画面遷移時に呼び出す関数を削除します。</summary>
	void RemoveHandler();
#pragma endregion
};

#pragma endregion



/// <summary>画面を管理します。</summary>
class SceneManager {
	Scene *CurrentScene;
	SceneType CurrentSceneType;
	SceneType NextScene;

	void SceneChange();

	SceneManager(SceneManager&);
	SceneManager &operator=(SceneManager&);
public:
	/// <summary>新しい SceneManager オブジェクトを初期化します。</summary>
	SceneManager();

	/// <summary>新しい SceneManager オブジェクトを初期化します。</summary>
	/// <param name="nextScene">最初のシーンを表す SceneType 列挙体の値</param>
	/// <example>
	/// 使用例:
	/// 注意！
	/// このサンプルでは、関数を使うための最小限の部分だけ紹介しています。
	/// ファイル "SceneBase.h" に示したサンプルも参考にしながら読んでください。
	/// <code>
	/// // 最初のシーンをタイトルシーンに設定
	///	SceneManager MySceneManager { ST_Title };
	/// </code>
	/// <example>
	SceneManager(SceneType nextScene);
	~SceneManager();

	/// <summary>シーンを更新します。</summary>
	void Update();
	/// <summary>シーンを描画します。</summary>
	void Draw() const;

	/// <summary>シーンを変更します。</summary>
	/// <param name="nextScene">次のシーンを表す SceneType 列挙体の値</param>
	/// <example>
	/// 使用例:
	/// 注意！
	/// このサンプルでは、関数を使うための最小限の部分だけ紹介しています。
	/// ファイル "SceneBase.h" に示したサンプルも参考にしながら読んでください。
	/// <code>
	///	SceneManager MySceneManager;
	/// // タイトルシーンに切り替え
	///	MySceneManager.OnSceneChange(ST_Title);
	/// </code>
	/// <example>
	void OnSceneChange(SceneType nextScene);
};