#pragma once

#include <map>

/// <summary>ジョイパッド用の強制終了コマンド</summary>
extern const int ExitJoyPadMask;

/// @file InputManager.h
/// InputManager.h -------------------------------------------------
/// エミュレート機能を備えた、ジョイパッド入力対応の入力管理クラス。
/// 最終更新:
///			2016/08/13
/// ----------------------------------------------------------------
/// @todo
/// 入力の抽象化によって通常の入力、保存した入力記録ファイルからの入力、AIの入力、ネットワーク対戦での他プレイヤーからの入力といったあらゆる入力を
/// 一元的に取得できるようにする予定。

// イベントドリブン型のエミュレート用キー入力データ。
struct EmulateKeyData {
	int Flame, PressSwitch;
	int KeyCode;
};

/// <summary>簡易アクセス用マクロ</summary>
#define InputMgr InputManager::Instance()

/// <summary>エミュレート機能を備えた、ジョイパッド入力対応の入力管理クラス。</summary>
class InputManager {
	// 入力データ
	char Buffer[256];
	int PressedTime[256];

	// エミュレート用データ
	bool IsEmulating;
	char EmulateBuffer[256];
	int EmulatePressedTime[256];

	// ジョイパッド入力データ
	int JoyPadInputBuffer;
	std::map<int, int> KeyTranslateData;

	// 更新データ
	int Flames;
	int KeyDataIndex, KeyDataNum;
	EmulateKeyData *KeyData;

	// インスタンス化を禁止
	InputManager();
	InputManager(const InputManager&);
	InputManager &operator=(const InputManager&);
public:
	static InputManager &Instance() {
		static InputManager inst;
		return inst;
	}

	~InputManager();

	/// <summary>内部データを初期化します。アプリケーション開始時は自動的に一度呼ばれるので、アプリケーション開始時に呼び出す必要はありません。</summary>
	void InitializeData();	
	/// <summary>入力状態を更新します。１フレームを目安に呼び出してください。</summary>
	void Update();	

	/// <summary>エミュレートファイルを読み込みます。</summary>
	bool LoadEmulateFile(const char filename[]);
	/// <summary>
	/// 入力のエミュレートを開始します。
	/// KeyPressed, KeyPressedOnce, KeyPressedFlame 関数に影響します。
	/// </summary>
	void StartEmulate();	
	/// <summary>入力のエミュレートを終了します。</summary>
	void EndEmulate();								

	/// <summary>特定のキー入力状態を取得します。</summary>
	bool KeyPressed(int keycode) const;
	/// <summary>特定のキー入力状態を取得します。タイピング向けです。</summary>
	bool KeyTyped(int keycode) const;
	/// <summary>特定のジョイパッド入力状態を取得します。</summary>
	int JoyPadPressed(int keycode);
	/// <summary>特定のキーが押された瞬間かどうかを取得します。</summary>
	bool KeyPressedOnce(int keycode) const;
	/// <summary>特定のキーが押されているフレーム数を取得します。</summary>
	int KeyPressedFlame(int keycode) const;

	/// <summary>実際のキー入力状態を取得します。</summary>
	bool KeyPressed_Actual(int keycode) const;
	/// <summary>実際のキーが押された瞬間かどうかを取得します。</summary>
	bool KeyPressedOnce_Actual(int keycode) const;
	/// <summary>実際のキーが押されているフレーム数を取得します。</summary>
	int KeyPressedFlame_Actual(int keycode) const;
	
	/// <summary>
	/// [2015年合宿仕様準拠]
	/// リセットキーコマンド（[E][S][C] Or [↓][3][5][6][7][8]）の状態を取得します。 
	/// </summary>
	bool GetResetRequestStatus() const;
};