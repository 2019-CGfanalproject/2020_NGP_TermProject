#pragma once
class NetworkCommunicator
{
	SOCKET m_Socket;

public:
	void Initialize();
	void Connect(const char* ip_addr, const String& nickname);

	void SendChatting(const String& chatting);
	void SendReady();
	void SendPlayerState();
	void SendBomb();

	void ReceiveRobbyPacket();
	DWORD WINAPI ReceiveWorldData(LPVOID params);
};
