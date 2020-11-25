#pragma once

enum class CommunicateMessage {
	CONNECT,
	PLAYER_IDLE,
	PLAYER_UP,
	PLAYER_DOWN,
	PLAYER_LEFT,
	PLAYER_RIGHT,
	UPDATE,
};

class GameFramework;
class NetworkCommunicator
{
	GameFramework* m_Framework;
	SOCKET m_Socket;
	std::queue<CommunicateMessage> m_MessageQueue;

public:
	static DWORD WINAPI ServerMain(LPVOID communicator);
	void PushMessage(CommunicateMessage msg);
	void SetFramework(GameFramework* framework);

	void SendPlayerState(PlayerState state);
	void SendChatting(const String& chatting);
	void SendReady();
	void SendBomb();

private:
	void Initialize();
	void Connect(const char* ip_addr, const String& nickname);

	void TranselateMessage(CommunicateMessage msg);

	void ReceiveRobbyPacket();
	void ReceiveWorldData();
};
