#pragma once

#ifndef DX_LIB_H
#	error "Network.h" ���C���N���[�h����O�ɁA"DXLib.h" ���C���N���[�h���Ă��������B
#endif

/// @file Network.h
/// Network.h -------------------------------------------------
/// �l�b�g���[�N�ʐM�̊֐���p�ӂ��܂��B
/// 
/// ���� ======================================================
///		"Network.cpp" �ƈꏏ�Ƀv���W�F�N�g�ɒǉ����Ă��������B
/// ===========================================================
///
/// �ŏI�X�V:
///			2016/08/18
/// -----------------------------------------------------------





/// <summary>�ڑ����</summary>
enum ConnectionState {
	/// <summary>�ʐM�͐ؒf����Ă��܂��B</summary>
	CS_Disconnected,
	/// <summary>�ʐM���m�����Ă���r���ł�</summary>
	CS_Connecting,
	/// <summary>�ʐM���m������Ă��܂��B</summary>
	CS_Connected
};

/// <summary>�����֐�:�g�p���Ȃ��ł��������B</summary>
bool SendDataInternal(void* data, size_t datasize);
/// <summary>�����֐�:�g�p���Ȃ��ł��������B</summary>
bool ReceiveDataInternal(void* data, size_t datasize, int timeout = -1);

namespace NetworkServer {

	/// <summary>�T�[�o�[���J�n���A�ʐM���󂯕t�������Ԃɂ��܂��B</summary>
	/// <returns>���������false, ���s�����true</returns>
	/// <remarks>�Z�p�Ҍ���:�g�p����|�[�g�ԍ���, Utility.cpp���Őݒ肵�܂��B</remarks>
	bool StartServer();

	/// <summary>�T�[�o�[���~���A�ʐM��ؒf���܂��B</summary>
	/// <returns>���������false, ���s�����true</returns>
	bool StopServer();

	/// <summary>�T�[�o�[�̏�Ԃ��擾���܂��B</summary>
	ConnectionState GetNetworkState();

	/// <summary>�T�[�o�[�̏�Ԃ��X�V���܂��B</summary>
	/// <returns>�ُ킪�Ȃ����false, ����ȊO�̏ꍇ��true</returns>
	/// <remarks>�T�[�o�[���J�n���Ă���Ԃ́A1�t���[����ڈ��ɌĂяo���Ă��������B</remarks>
	bool Update();

	/// <summary>�f�[�^�𑗐M���܂��B</summary>
	/// <param name="data">���M����f�[�^</param>
	/// <returns>�ُ킪�Ȃ����false, ����ȊO�̏ꍇ��true</returns>
	template <typename Ty>
	bool SendData(Ty& data) {
		return SendDataInternal(&data, sizeof(data));
	}

	/// <summary>�f�[�^����M���܂��B</summary>
	/// <param name="data">��M����f�[�^</param>
	/// <param name="timeout">
	///		�ҋ@���鎞��(�~���b)�B���̊Ԃ͎�M�f�[�^�ʂ������悤�ɂȂ�܂Ŏ�M�𑱂��܂��B
	///		�w�肵�Ȃ���΁A��x�̂ݎ�M�����݂܂��B
	/// </param>
	/// <returns>�ُ킪�Ȃ����false, ����ȊO�̏ꍇ��true</returns>
	template <typename Ty>
	bool ReceiveData(Ty& data, int timeout = -1) {
		return ReceiveDataInternal(&data, sizeof(data), timeout);
	}
}

namespace NetworkClient {

	/// <summary>�T�[�o�[�Ƃ̒ʐM���m�����܂��B</summary>
	/// <returns>���������false, ���s�����true</returns>
	/// <remarks>
	///		�Z�p�Ҍ���:�g�p����|�[�g�ԍ���, Utility.cpp���Őݒ肵�܂��B
	///		�ڑ����IP�A�h���X�́AConnectionSetting.txt ���Q�Ƃ��Ă��������B
	/// </remarks>
	bool Connect();

	/// <summary>�T�[�o�[�Ƃ̒ʐM��ؒf���܂��B</summary>
	/// <returns>���������false, ���s�����true</returns>
	bool Disconnect();

	/// <summary>�l�b�g���[�N�̏�Ԃ��擾���܂��B</summary>
	ConnectionState GetNetworkState();

	/// <summary>�l�b�g���[�N�̏�Ԃ��X�V���܂��B</summary>
	/// <returns>�ُ킪�Ȃ����false, ����ȊO�̏ꍇ��true</returns>
	/// <remarks>�ʐM���m�����Ă���Ԃ́A1�t���[����ڈ��ɌĂяo���Ă��������B</remarks>
	bool Update();

	/// <summary>�f�[�^�𑗐M���܂��B</summary>
	/// <param name="data">���M����f�[�^</param>
	/// <returns>�ُ킪�Ȃ����false, ����ȊO�̏ꍇ��true</returns>
	template <typename Ty>
	bool SendData(Ty& data) {
		return SendDataInternal(&data, sizeof(data));
	}

	/// <summary>�f�[�^����M���܂��B</summary>
	/// <param name="data">��M����f�[�^</param>
	/// <returns>�ُ킪�Ȃ����false, ����ȊO�̏ꍇ��true</returns>
	template <typename Ty>
	bool ReceiveData(Ty& data, int timeout = -1) {
		return ReceiveDataInternal(&data, sizeof(data), timeout);
	}
}

namespace InternetClient {

	/// <summary>�T�[�o�[�Ƃ̒ʐM���m�����܂��B</summary>
	/// <return>���������false, ���s�����true</return>
	bool Connect();

	/// <summary>�T�[�o�[�Ƃ̒ʐM��ؒf���܂��B</summary>
	/// <return>���������false, ���s�����true</return>
	bool Disconnect();

	/// <summary>�l�b�g���[�N�̏�Ԃ��X�V���܂��B</summary>
	/// <return>�ُ킪�Ȃ����false, ����ȊO�̏ꍇ��true</return>
	/// <remarks>1�t���[����ڈ��ɌĂяo���Ă��������B</remarks>
	bool Update();

	/// <summary>�����֐�:�g�p���Ȃ��ł��������B</summary>
	bool SendDataInternal(void* data, size_t datasize);
	/// <summary>�����֐�:�g�p���Ȃ��ł��������B</summary>
	bool ReceiveDataInternal(void* data, size_t datasize, int timeout = -1);

	/// <summary>�f�[�^�𑗐M���܂��B</summary>
	/// <param name="data">���M����f�[�^</param>
	/// <return>�ُ킪�Ȃ����false, ����ȊO�̏ꍇ��true</return>
	template <typename Ty>
	bool SendData(Ty& data) {
		return SendDataInternal(&data, sizeof(data));
	};

	/// <summary>�f�[�^����M���܂��B</summary>
	/// <param name="data">��M����f�[�^</param>
	/// <return>�ُ킪�Ȃ����false, ����ȊO�̏ꍇ��true</return>
	template <typename Ty>
	bool ReceiveData(Ty& data, int timeout = -1) {
		return ReceiveDataInternal(&data, sizeof(data), timeout);
	};
}