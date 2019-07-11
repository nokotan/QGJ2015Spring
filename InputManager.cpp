#include "InputManager.h"
#include "DebugHelper.h"
#include "DxLib.h"

const int ExitJoyPadMask = PAD_INPUT_DOWN | PAD_INPUT_3 | PAD_INPUT_5 | PAD_INPUT_6 | PAD_INPUT_7 | PAD_INPUT_8;

using namespace std;

InputManager::InputManager() :
	IsEmulating(false), Flames(-1),
	KeyDataIndex(0), KeyDataNum(0) {
	
	for (int i = 0; i < 256; i++)
		EmulateBuffer[i] = 0;
}

InputManager::~InputManager() {
	if (KeyDataNum != 0) {
		delete[] KeyData;
	}
}

void InputManager::InitializeData() {
	for (int i = 0; i < 256; i++)
		PressedTime[i] = 0;

	for (int i = 0; i < 256; i++)
		EmulatePressedTime[i] = 0;
	
	KeyTranslateData[KEY_INPUT_Z] = PAD_INPUT_1;
	KeyTranslateData[KEY_INPUT_X] = PAD_INPUT_2;
	KeyTranslateData[KEY_INPUT_ESCAPE] = PAD_INPUT_9;
	KeyTranslateData[KEY_INPUT_UP] = PAD_INPUT_UP;
	KeyTranslateData[KEY_INPUT_DOWN] = PAD_INPUT_DOWN;
	KeyTranslateData[KEY_INPUT_LEFT] = PAD_INPUT_LEFT;
	KeyTranslateData[KEY_INPUT_RIGHT] = PAD_INPUT_RIGHT;
}

void InputManager::Update() {
	GetHitKeyStateAll(Buffer);
	JoyPadInputBuffer = GetJoypadInputState(DX_INPUT_PAD1);

	// ジョイパッド入力をキー入力に変換
	for (map<int, int>::iterator iter = KeyTranslateData.begin();
		iter != KeyTranslateData.end();
		iter++) {

		if (iter->second & JoyPadInputBuffer)
			Buffer[iter->first]++;
	}

	for (int i = 0; i < 256; i++) {
		if (Buffer[i])
			PressedTime[i]++;
		else
			PressedTime[i] = 0;
	}

	if (IsEmulating) {
		Flames++;

		for (int i = KeyDataIndex; i < KeyDataNum; i++) {
			if (KeyData[i].Flame == Flames) {
				EmulateBuffer[KeyData[i].KeyCode] = KeyData[i].PressSwitch;
				KeyDataIndex = i;
			}
			else if (KeyData[i].Flame > Flames) {
				break;
			}
		}

		for (int i = 0; i < 256; i++) {
			if (EmulateBuffer[i])
				EmulatePressedTime[i]++;
			else
				EmulatePressedTime[i] = 0;
		}

		if (KeyDataIndex == KeyDataNum - 1)
			IsEmulating = false;
	}
}

int InputManager::JoyPadPressed(int keycode) {
	return (JoyPadInputBuffer & KeyTranslateData[keycode]);
}

bool InputManager::KeyPressed(int KeyCode) const {
#ifdef _DEBUG
	// 引数の値のチェック
	if (KeyCode < 0 || KeyCode >= 256) {
		DebugTrace("ArgumentOutOfRangeException: 引数 %s の値が無効です。", GetVariableName(KeyCode));
		return false;
	} else
#endif
	{
		return ((IsEmulating ? EmulatePressedTime[KeyCode] : PressedTime[KeyCode]) > 0);
	}
}

bool InputManager::KeyTyped(int KeyCode) const {
#ifdef _DEBUG
	if (KeyCode < 0 || KeyCode >= 256) {
		DebugTrace("ArgumentOutOfRangeException: 引数 %s の値が無効です。", GetVariableName(KeyCode));
		return false;
	} else 
#endif
	{
		return (IsEmulating ?
					EmulatePressedTime[KeyCode] == 1	|| (EmulatePressedTime[KeyCode] >= 40	&& !(EmulatePressedTime[KeyCode] % 3)) :
					PressedTime[KeyCode] == 1			|| (PressedTime[KeyCode] >= 40			&& !(PressedTime[KeyCode] % 3)));
	}
}

bool InputManager::KeyPressedOnce(int KeyCode) const {
#ifdef _DEBUG
	if (KeyCode < 0 || KeyCode >= 256) {
		DebugTrace("ArgumentOutOfRangeException: 引数 %s の値が無効です。", GetVariableName(KeyCode));
		return false;
	} else 
#endif
	{
		return ((IsEmulating ? EmulatePressedTime[KeyCode] : PressedTime[KeyCode]) == 1);
	}
}

int InputManager::KeyPressedFlame(int KeyCode) const {
#ifdef _DEBUG
	if (KeyCode < 0 || KeyCode >= 256) {
		DebugTrace("ArgumentOutOfRangeException: 引数 %s の値が無効です。", GetVariableName(KeyCode));
		return false;
	} else 
#endif
	{
		return (IsEmulating ? EmulatePressedTime[KeyCode] : PressedTime[KeyCode]);
	}
}

bool InputManager::LoadEmulateFile(const char filename[]) {

	if (KeyDataNum != 0)
		delete[] KeyData;

	int h_File = FileRead_open(filename);

	if (!h_File)
		return false;

	char buffer[256];

	// 大きさデータの読み込み
	FileRead_gets(buffer, 256, h_File);
	sscanf_s(buffer, "%d", &KeyDataNum);

	KeyData = new EmulateKeyData[KeyDataNum];

	int flame, key, pressed;

	for (int i = 0; i < KeyDataNum; i++) {
		FileRead_gets(buffer, 256, h_File);
		sscanf_s(buffer, "%d,%d,%d,", &flame, &key, &pressed);

		KeyData[i].Flame = flame;
		KeyData[i].KeyCode = key;
		KeyData[i].PressSwitch = pressed;
	}

	FileRead_close(h_File);

	return true;
}

void InputManager::StartEmulate() {
	IsEmulating = true;
	Flames = -1;
	KeyDataIndex = 0;

	for (int i = 0; i < 256; i++)
		EmulateBuffer[i] = 0;
}

void InputManager::EndEmulate() {
	IsEmulating = false;
}

bool InputManager::KeyPressed_Actual(int KeyCode) const {
#ifdef _DEBUG
	if (KeyCode < 0 || KeyCode >= 256) {
		DebugTrace("ArgumentOutOfRangeException: 引数 %s の値が無効です。", GetVariableName(KeyCode));
		return false;
	} else
#endif
		return (PressedTime[KeyCode] > 0);
}

bool InputManager::KeyPressedOnce_Actual(int KeyCode) const {
#ifdef _DEBUG
	if (KeyCode < 0 || KeyCode >= 256) {
		DebugTrace("ArgumentOutOfRangeException: 引数 %s の値が無効です。", GetVariableName(KeyCode));
		return false;
	} else
#endif
		return (PressedTime[KeyCode] == 1);
}

int InputManager::KeyPressedFlame_Actual(int KeyCode) const {
#ifdef _DEBUG
	if (KeyCode < 0 || KeyCode >= 256) {
		DebugTrace("ArgumentOutOfRangeException: 引数 %s の値が無効です。", GetVariableName(KeyCode));
		return 0;
	} else
#endif
		return PressedTime[KeyCode];
}

bool InputManager::GetResetRequestStatus() const {
	bool KeyRequest = (PressedTime[KEY_INPUT_E] > 0 && PressedTime[KEY_INPUT_S] > 0 && PressedTime[KEY_INPUT_C] > 0);
	bool PadRequest = (JoyPadInputBuffer & ExitJoyPadMask) == ExitJoyPadMask;

	return (KeyRequest | PadRequest);
}