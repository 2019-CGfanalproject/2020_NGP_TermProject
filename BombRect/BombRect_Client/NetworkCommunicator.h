#pragma once

enum class NETWORK_MASSAGE {
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
	std::queue<NETWORK_MASSAGE> m_MessageQueue;

public:
	static DWORD WINAPI ServerMain(LPVOID communicator);
	void PushMessage(NETWORK_MASSAGE msg);

	void SetFramework(GameFramework* framework);

private:
	void Initialize();
	void Connect(const char* ip_addr, const String& nickname);

	void TranselateMessage(NETWORK_MASSAGE msg);

	void SendChatting(const String& chatting);
	void SendReady();
	void SendPlayerState(PlayerState state);
	void SendBomb();

	void ReceiveRobbyPacket();
	void ReceiveWorldData();
};
