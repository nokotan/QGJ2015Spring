#pragma once

#ifndef DX_LIB_H
#	error "Network.h" をインクルードする前に、"DXLib.h" をインクルードしてください。
#endif

/// @file Network.h
/// Network.h -------------------------------------------------
/// ネットワーク通信の関数を用意します。
/// 
/// 注意 ======================================================
///		"Network.cpp" と一緒にプロジェクトに追加してください。
/// ===========================================================
///
/// 最終更新:
///			2016/08/18
/// -----------------------------------------------------------





/// <summary>接続状態</summary>
enum ConnectionState {
	/// <summary>通信は切断されています。</summary>
	CS_Disconnected,
	/// <summary>通信を確立している途中です</summary>
	CS_Connecting,
	/// <summary>通信が確立されています。</summary>
	CS_Connected
};

/// <summary>内部関数:使用しないでください。</summary>
bool SendDataInternal(void* data, size_t datasize);
/// <summary>内部関数:使用しないでください。</summary>
bool ReceiveDataInternal(void* data, size_t datasize, int timeout = -1);

namespace NetworkServer {

	/// <summary>サーバーを開始し、通信を受け付けられる状態にします。</summary>
	/// <returns>成功すればfalse, 失敗すればtrue</returns>
	/// <remarks>技術者向け:使用するポート番号は, Utility.cpp内で設定します。</remarks>
	bool StartServer();

	/// <summary>サーバーを停止し、通信を切断します。</summary>
	/// <returns>成功すればfalse, 失敗すればtrue</returns>
	bool StopServer();

	/// <summary>サーバーの状態を取得します。</summary>
	ConnectionState GetNetworkState();

	/// <summary>サーバーの状態を更新します。</summary>
	/// <returns>異常がなければfalse, それ以外の場合はtrue</returns>
	/// <remarks>サーバーを開始している間は、1フレームを目安に呼び出してください。</remarks>
	bool Update();

	/// <summary>データを送信します。</summary>
	/// <param name="data">送信するデータ</param>
	/// <returns>異常がなければfalse, それ以外の場合はtrue</returns>
	template <typename Ty>
	bool SendData(Ty& data) {
		return SendDataInternal(&data, sizeof(data));
	}

	/// <summary>データを受信します。</summary>
	/// <param name="data">受信するデータ</param>
	/// <param name="timeout">
	///		待機する時間(ミリ秒)。この間は受信データ量が足りるようになるまで受信を続けます。
	///		指定しなければ、一度のみ受信を試みます。
	/// </param>
	/// <returns>異常がなければfalse, それ以外の場合はtrue</returns>
	template <typename Ty>
	bool ReceiveData(Ty& data, int timeout = -1) {
		return ReceiveDataInternal(&data, sizeof(data), timeout);
	}
}

namespace NetworkClient {

	/// <summary>サーバーとの通信を確立します。</summary>
	/// <returns>成功すればfalse, 失敗すればtrue</returns>
	/// <remarks>
	///		技術者向け:使用するポート番号は, Utility.cpp内で設定します。
	///		接続先のIPアドレスは、ConnectionSetting.txt を参照してください。
	/// </remarks>
	bool Connect();

	/// <summary>サーバーとの通信を切断します。</summary>
	/// <returns>成功すればfalse, 失敗すればtrue</returns>
	bool Disconnect();

	/// <summary>ネットワークの状態を取得します。</summary>
	ConnectionState GetNetworkState();

	/// <summary>ネットワークの状態を更新します。</summary>
	/// <returns>異常がなければfalse, それ以外の場合はtrue</returns>
	/// <remarks>通信を確立している間は、1フレームを目安に呼び出してください。</remarks>
	bool Update();

	/// <summary>データを送信します。</summary>
	/// <param name="data">送信するデータ</param>
	/// <returns>異常がなければfalse, それ以外の場合はtrue</returns>
	template <typename Ty>
	bool SendData(Ty& data) {
		return SendDataInternal(&data, sizeof(data));
	}

	/// <summary>データを受信します。</summary>
	/// <param name="data">受信するデータ</param>
	/// <returns>異常がなければfalse, それ以外の場合はtrue</returns>
	template <typename Ty>
	bool ReceiveData(Ty& data, int timeout = -1) {
		return ReceiveDataInternal(&data, sizeof(data), timeout);
	}
}

namespace InternetClient {

	/// <summary>サーバーとの通信を確立します。</summary>
	/// <return>成功すればfalse, 失敗すればtrue</return>
	bool Connect();

	/// <summary>サーバーとの通信を切断します。</summary>
	/// <return>成功すればfalse, 失敗すればtrue</return>
	bool Disconnect();

	/// <summary>ネットワークの状態を更新します。</summary>
	/// <return>異常がなければfalse, それ以外の場合はtrue</return>
	/// <remarks>1フレームを目安に呼び出してください。</remarks>
	bool Update();

	/// <summary>内部関数:使用しないでください。</summary>
	bool SendDataInternal(void* data, size_t datasize);
	/// <summary>内部関数:使用しないでください。</summary>
	bool ReceiveDataInternal(void* data, size_t datasize, int timeout = -1);

	/// <summary>データを送信します。</summary>
	/// <param name="data">送信するデータ</param>
	/// <return>異常がなければfalse, それ以外の場合はtrue</return>
	template <typename Ty>
	bool SendData(Ty& data) {
		return SendDataInternal(&data, sizeof(data));
	};

	/// <summary>データを受信します。</summary>
	/// <param name="data">受信するデータ</param>
	/// <return>異常がなければfalse, それ以外の場合はtrue</return>
	template <typename Ty>
	bool ReceiveData(Ty& data, int timeout = -1) {
		return ReceiveDataInternal(&data, sizeof(data), timeout);
	};
}