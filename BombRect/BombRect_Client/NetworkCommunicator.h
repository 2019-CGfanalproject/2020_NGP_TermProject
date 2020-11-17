#pragma once

// 너무 Window의 구조체스러움;;
enum class NETWORK_MASSAGE {
	CONNECT,
};

class NetworkCommunicator
{
	SOCKET m_Socket;
	std::queue<NETWORK_MASSAGE> m_MessageQueue;

public:
	static DWORD WINAPI ServerMain(LPVOID communicator);
	void PushMessage(NETWORK_MASSAGE msg);

private:
	void Initialize();
	void Connect(const char* ip_addr, const String& nickname);

	void TranselateMessage(NETWORK_MASSAGE msg);

	void SendChatting(const String& chatting);
	void SendReady();
	void SendPlayerState();
	void SendBomb();

	void ReceiveRobbyPacket();
	DWORD WINAPI ReceiveWorldData(LPVOID params);
};
