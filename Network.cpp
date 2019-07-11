#include "DxLib.h"
// 上で赤波線が出る場合は、以下の文と入れ替えてください。
// #include <DxLib.h>

/// <summary>接続に使用するポート番号</summary>
static int PortNo = 55055;


// ==================================================
// これより下は、必要なければ書き換えないでください。
// ==================================================

#include "Network.h"
#pragma warning(push)
#pragma warning(disable:4267)

static IPDATA IPData = { 127, 0, 0, 1 };
static int NetworkHandle = -1;
static ConnectionState InternalState = ConnectionState::CS_Disconnected;

bool SendDataInternal(void* data, size_t datasize) {
	if (InternalState != CS_Connected) return true;
	return (NetWorkSend(NetworkHandle, data, datasize) == -1);
}

bool ReceiveDataInternal(void* data, size_t datasize, int timeout) {
	if (InternalState != CS_Connected) return true;

	if (timeout == -1)
		return (NetWorkRecv(NetworkHandle, data, datasize) == -1);
	else {
		int StartTick = GetNowCount();

		while (GetNowCount() - StartTick < timeout) {
			if (GetNetWorkDataLength(NetworkHandle) >= datasize) {
				return NetWorkRecv(NetworkHandle, data, datasize) == -1;
			}
			Sleep(0);
		}

		return true;
	}
}

namespace NetworkClient {
	bool Connect() {
		if (InternalState != CS_Disconnected) {
			return true;
		} else {
			int FileHandle = FileRead_open("ConnectionSetting.txt");
			
			if (FileHandle == -1) {
				OutputDebugString("ConnectionSetting.txt が見つかりませんでした。\n");
				return true;
			}
			
			int IP[4];

			if (FileRead_scanf(FileHandle, "IP = %d.%d.%d.%d", IP, IP + 1, IP + 2, IP + 3) == -1) {
				OutputDebugString("ConnectionSetting.txt からIPアドレスが読みだせませんでした。\n");
				FileRead_close(FileHandle);
				return true;
			}

			IPData.d1 = static_cast<unsigned char>(IP[0]);
			IPData.d2 = static_cast<unsigned char>(IP[1]);
			IPData.d3 = static_cast<unsigned char>(IP[2]);
			IPData.d4 = static_cast<unsigned char>(IP[3]);

			FileRead_close(FileHandle);

			if ((NetworkHandle = ConnectNetWork(IPData, PortNo)) == -1) {
				OutputDebugString("サーバーに接続できませんでした。\n");
				return true;
			};

			InternalState = CS_Connected;
			return false;
		}
	}

	bool Disconnect() {
		if (InternalState != CS_Connected)
			return true;

		InternalState = CS_Disconnected;
		CloseNetWork(NetworkHandle);
		return false;
	}

	ConnectionState GetNetworkState() {
		return InternalState;
	}

	bool Update() {
		switch (InternalState) {
			case CS_Connected:
				if (NetworkHandle == GetLostNetWork()) {
					Disconnect();
					printf("disconnected");
					return true;
				}
				break;
		}
		return false;
	}
}

namespace NetworkServer {

	bool StartServer() {
		GetMyIPAddress(&IPData);

		if (InternalState != CS_Disconnected) 
			return true;
		else {
			PreparationListenNetWork(PortNo);
			InternalState = ConnectionState::CS_Connecting;
			return false;
		}
	}

	bool StopServer() {
		ConnectionState prevInternalState = InternalState;
		InternalState = CS_Disconnected;

		switch (prevInternalState) {
			case CS_Disconnected:
				return true;
				break;
			case CS_Connecting:
				return (StopListenNetWork() == -1);
				break;
			case CS_Connected:
				return (CloseNetWork(NetworkHandle) == -1);
				break;
		}
		return false;
	}

	ConnectionState GetNetworkState() {
		return InternalState;
	}

	bool Update() {
		switch (InternalState) {
			case CS_Connecting:
				if ((NetworkHandle = GetNewAcceptNetWork()) != -1) {
					InternalState = CS_Connected;
				}
				break;
			case CS_Connected:
				if (NetworkHandle == GetLostNetWork()) {
					StopServer();
					printf("ServerStopped");
					return true;
				}
				break;
		}
		return false;
	}
}

#if defined(USE_INTERNET_CONNECTION)

#include "Client.h"
#include <new>
#include <deque>

namespace InternetClient {

	using namespace ExitGames;

	static Common::JString ApplicationID = L"129b9b87-f353-4fa8-9d82-55a145b94b78";

	struct DataPack {
		nByte *pData;
		size_t DataSize;

		DataPack(nByte *pdata, size_t size) :
			pData(pdata), DataSize(size) {
		}
	};

	class PhotonNetworkProxy : public LoadBalancing::Listener {
		std::deque<DataPack> ReceivedData;

		LoadBalancing::Client mClient;

		// デバッグメッセージを受け取ります。
		virtual void debugReturn(int debugLevel, const Common::JString& string) {};

		// エラーが発生した時のハンドラー
		virtual void connectionErrorReturn(int errorCode) {};
		virtual void clientErrorReturn(int errorCode) {};
		virtual void warningReturn(int warningCode) {};
		virtual void serverErrorReturn(int errorCode) {};

		// 各種イベントのハンドラー
		virtual void joinRoomEventAction(int playerNr, const Common::JVector<int>& playernrs, const LoadBalancing::Player& player) {};
		virtual void leaveRoomEventAction(int playerNr, bool isInactive) {};
		virtual void customEventAction(int playerNr, nByte eventCode, const Common::Object& eventContent) {
			Common::Hashtable DataContent = Common::ValueObject<Common::Hashtable>(eventContent).getDataCopy();

			switch (eventCode) {
			case 0u:
				ReceivedData.push_back(DataPack(Common::ValueObject<nByte*>(DataContent.getValue(static_cast<nByte>(1u))).getDataCopy(), Common::ValueObject<nByte*>(DataContent.getValue(static_cast<nByte>(1u))).getSizes()[0]));
			}
		};

		// サーバーからの応答
		virtual void connectReturn(int errorCode, const Common::JString& errorString) { OutputDebugStringW(errorString.cstr()); mClient.opJoinOrCreateRoom(L"10"); };
		virtual void disconnectReturn(void) {};
		virtual void createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties, const Common::Hashtable& playerProperties, int errorCode, const Common::JString& errorString) { OutputDebugStringW(errorString.cstr()); };
		virtual void joinRoomReturn(int localPlayerNr, const Common::Hashtable& roomProperties, const Common::Hashtable& playerProperties, int errorCode, const Common::JString& errorString) { OutputDebugStringW(errorString.cstr()); };
		virtual void joinRandomRoomReturn(int localPlayerNr, const Common::Hashtable& roomProperties, const Common::Hashtable& playerProperties, int errorCode, const Common::JString& errorString) { OutputDebugStringW(errorString.cstr()); };
		virtual void joinOrCreateRoomReturn(int localPlayerNr, const Common::Hashtable& roomProperties, const Common::Hashtable& playerProperties, int errorCode, const Common::JString& errorString) { OutputDebugStringW(errorString.cstr()); };
		virtual void leaveRoomReturn(int errorCode, const Common::JString& errorString) { OutputDebugStringW(errorString.cstr()); };
		virtual void joinLobbyReturn(void) {  };
		virtual void leaveLobbyReturn(void) {};
	public:
		PhotonNetworkProxy();

		void Update() {
			mClient.service();
		}

		void Connect() {
			mClient.connect();
		}

		void Disconnect() {
			mClient.disconnect();
		}

		void SendData(Common::Hashtable& data) {
			mClient.opRaiseEvent(true, data, 0);
		}

		bool ReceiveData(char* data, size_t size) {
			if (ReceivedData.size() == 0) {
				return true;
			}

			DataPack pack = ReceivedData.front();

			if (size == pack.DataSize) {
				return true;
			}

			memcpy_s(data, size, pack.pData, pack.DataSize);
			Common::MemoryManagement::deallocateArray(pack.pData);
			ReceivedData.pop_front();
			return false;
		}
	} PhotonProxy;

	PhotonNetworkProxy::PhotonNetworkProxy() :
		mClient(*this, ApplicationID, "1.0") {
	}

	bool Connect() {
		PhotonProxy.Connect();
		return false;
	}

	bool Disconnect() {
		PhotonProxy.Disconnect();
		return false;
	}

	bool Update() {
		PhotonProxy.Update();
		return false;
	}

	bool ReceiveDataInternal(void* data, size_t datasize, int timeout) {
		return PhotonProxy.ReceiveData(static_cast<char*>(data), datasize);
	}

	bool SendDataInternal(void* data, size_t datasize) {
		Common::Hashtable DataPack;
		DataPack.put(static_cast<nByte>(1U), static_cast<nByte*>(data), datasize);
		PhotonProxy.SendData(DataPack);

		return false;
	}
}

#endif

#pragma warning(pop)