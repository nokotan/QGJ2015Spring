#pragma once

#include <map>

/// <summary>�W���C�p�b�h�p�̋����I���R�}���h</summary>
extern const int ExitJoyPadMask;

/// @file InputManager.h
/// InputManager.h -------------------------------------------------
/// �G�~�����[�g�@�\��������A�W���C�p�b�h���͑Ή��̓��͊Ǘ��N���X�B
/// �ŏI�X�V:
///			2016/08/13
/// ----------------------------------------------------------------
/// @todo
/// ���͂̒��ۉ��ɂ���Ēʏ�̓��́A�ۑ��������͋L�^�t�@�C������̓��́AAI�̓��́A�l�b�g���[�N�ΐ�ł̑��v���C���[����̓��͂Ƃ�������������͂�
/// �ꌳ�I�Ɏ擾�ł���悤�ɂ���\��B

// �C�x���g�h���u���^�̃G�~�����[�g�p�L�[���̓f�[�^�B
struct EmulateKeyData {
	int Flame, PressSwitch;
	int KeyCode;
};

/// <summary>�ȈՃA�N�Z�X�p�}�N��</summary>
#define InputMgr InputManager::Instance()

/// <summary>�G�~�����[�g�@�\��������A�W���C�p�b�h���͑Ή��̓��͊Ǘ��N���X�B</summary>
class InputManager {
	// ���̓f�[�^
	char Buffer[256];
	int PressedTime[256];

	// �G�~�����[�g�p�f�[�^
	bool IsEmulating;
	char EmulateBuffer[256];
	int EmulatePressedTime[256];

	// �W���C�p�b�h���̓f�[�^
	int JoyPadInputBuffer;
	std::map<int, int> KeyTranslateData;

	// �X�V�f�[�^
	int Flames;
	int KeyDataIndex, KeyDataNum;
	EmulateKeyData *KeyData;

	// �C���X�^���X�����֎~
	InputManager();
	InputManager(const InputManager&);
	InputManager &operator=(const InputManager&);
public:
	static InputManager &Instance() {
		static InputManager inst;
		return inst;
	}

	~InputManager();

	/// <summary>�����f�[�^�����������܂��B�A�v���P�[�V�����J�n���͎����I�Ɉ�x�Ă΂��̂ŁA�A�v���P�[�V�����J�n���ɌĂяo���K�v�͂���܂���B</summary>
	void InitializeData();	
	/// <summary>���͏�Ԃ��X�V���܂��B�P�t���[����ڈ��ɌĂяo���Ă��������B</summary>
	void Update();	

	/// <summary>�G�~�����[�g�t�@�C����ǂݍ��݂܂��B</summary>
	bool LoadEmulateFile(const char filename[]);
	/// <summary>
	/// ���͂̃G�~�����[�g���J�n���܂��B
	/// KeyPressed, KeyPressedOnce, KeyPressedFlame �֐��ɉe�����܂��B
	/// </summary>
	void StartEmulate();	
	/// <summary>���͂̃G�~�����[�g���I�����܂��B</summary>
	void EndEmulate();								

	/// <summary>����̃L�[���͏�Ԃ��擾���܂��B</summary>
	bool KeyPressed(int keycode) const;
	/// <summary>����̃L�[���͏�Ԃ��擾���܂��B�^�C�s���O�����ł��B</summary>
	bool KeyTyped(int keycode) const;
	/// <summary>����̃W���C�p�b�h���͏�Ԃ��擾���܂��B</summary>
	int JoyPadPressed(int keycode);
	/// <summary>����̃L�[�������ꂽ�u�Ԃ��ǂ������擾���܂��B</summary>
	bool KeyPressedOnce(int keycode) const;
	/// <summary>����̃L�[��������Ă���t���[�������擾���܂��B</summary>
	int KeyPressedFlame(int keycode) const;

	/// <summary>���ۂ̃L�[���͏�Ԃ��擾���܂��B</summary>
	bool KeyPressed_Actual(int keycode) const;
	/// <summary>���ۂ̃L�[�������ꂽ�u�Ԃ��ǂ������擾���܂��B</summary>
	bool KeyPressedOnce_Actual(int keycode) const;
	/// <summary>���ۂ̃L�[��������Ă���t���[�������擾���܂��B</summary>
	int KeyPressedFlame_Actual(int keycode) const;
	
	/// <summary>
	/// [2015�N���h�d�l����]
	/// ���Z�b�g�L�[�R�}���h�i[E][S][C] Or [��][3][5][6][7][8]�j�̏�Ԃ��擾���܂��B 
	/// </summary>
	bool GetResetRequestStatus() const;
};