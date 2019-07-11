#pragma once

/**
* @file GameResourseManager.h
* GameResourseManager.h ------------------------
* ゲーム中で使用する音楽や画像などを管理します。
* ----------------------------------------------
*/

#include "DebugHelper.h"

/// <summary>画像を識別する定数</summary>
/// <remarks>必要に応じて要素を追加してください。</remarks>
enumdef(GameImages,
	Player,
	Enemy,
	Neutral
)

/// <summary>音楽を識別する定数</summary>
/// <remarks>必要に応じて要素を追加してください。</remarks>
enumdef(GameMusics,

)

/// <summary>簡易アクセス用マクロ</summary>
#define GameResourseMgr GameResourseManager::Instance()

/// <summary>
/// ゲーム中で使用する音楽や画像などを管理します。
/// [Singleton クラス] 
/// インスタンス生成ができないので、GameResourseManager::Instance() 関数 または GameResourseMgr マクロを使ってインスタンスにアクセスしてください。
/// </summary>
/// <example>
/// 使用例:
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
///	// 以下編集の必要がないので省略
/// </code>
///	GameResourseManager.cpp
/// <code>
/// #include "GameResourseManager.h"
/// #include "DxLib.h"
///
/// #include &lt;string&gt;
///
/// 画像の読み込みに必要なデータを格納します。
/// データは、LoadDivGraph に使うものです。
/// struct GraphData {
///		// 定義は省略	
/// };
///
/// // プレイヤー、敵、アイテムの画像の名前がそれぞれ、"Player.png", "Enemy.png", "Item.png"
/// // である時の書き方です。実際に使うファイル名に応じて書き換えてください。
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
///	// 以下編集の必要がないので省略
/// </code>
/// 使用部分
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
///		// メインループ
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

	// インスタンス生成およびコピー禁止
	GameResourseManager();
	GameResourseManager(const GameResourseManager&);
	GameResourseManager &operator=(const GameResourseManager&);
public:
	~GameResourseManager();

	/// <summary>GameResourseManager インスタンスを取得します。</summary>
	/// <returns>GameResourseManager インスタンスへの参照</returns>
	static GameResourseManager &Instance() {
		static GameResourseManager instance;
		return instance;
	}
	
	/// <summary>画像を初期化します。</summary>
	/// <returns>初期化に成功すれば false, そうでなければ true</returns>
	bool InitializeImages();
	/// <summary>画像を明示的に解放します。</summary>
	void DisposeImages();
	/// <summary>音楽を初期化します。</summary>
	/// <returns>初期化に成功すれば false, そうでなければ true</returns>
	bool InitializeMusics();
	/// <summary>音楽を明示的に解放します。</summary>
	void DisposeMusics();

	/// <summary>指定した画像のハンドルを取得します。</summary>
	int GetGameImageHandle(GameImages);
	/// <summary>指定した画像のハンドルを取得します。</summary>
	int *GetGameImageHandleArray(GameImages);
	/// <summary>指定した画像の数を取得します。</summary>
	int GetGameImageHandleCount(GameImages);
	/// <summary>指定した音楽のハンドルを取得します。</summary>
	int GetGameMusicHandle(GameMusics);
};

// 指定したウィンドウの描画内容をキャプチャします。
int ScreenCapture(struct HWND__* hwnd = nullptr);

/// <summary>新しく画像を読み込み、アセットマネージャにそのハンドルを登録します。</summary>
/// <param name="FileName">新しく読み込む画像ファイルのパス</param>
/// <returns>登録に成功すればそのグラフィックハンドル, そうでなければ -1</returns>
/// <remarks>すでに登録名が使われていた場合は何もせず、関連付けられたグラフィックハンドルを返します。</remarks>
int LoadGraphToAsset(const char FileName[]);

/// <summary>新しく音声を読み込み、アセットマネージャにそのハンドルを登録します。</summary>
/// <param name="FileName">新しく読み込む音声ファイルのパス</param>
/// <returns>登録に成功すればその音声ハンドル, そうでなければ -1</returns>
/// <remarks>すでに登録名が使われていた場合は何もせず、関連付けられた音声ハンドルを返します。</remarks>
int LoadSoundMemToAsset(const char FileName[]);

/// <summary>アセットマネージャから指定した登録名に関連付けられている画像や音声のハンドルを取り出します。</summary>
/// <param name="FileName">すでに読み込んでいる画像や音声ファイルのパス</param>
/// <returns>ハンドルの取得に成功すれば、そのハンドル。そうでなければ、-1</returns>
int GetHandleToAsset(const char FileName[]);