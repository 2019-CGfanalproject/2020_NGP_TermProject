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

	SceneID m_CurScene;

public:
	void Initialize(GameFramework* framework);

	bool Connect(const char* ip_addr, const String& nickname);
	void SendReady();
	void SendChatting(const String& chatting);
	void SendPlayerState(PlayerState state);
	void SendBomb();

private:
	static DWORD WINAPI ReceiveProc(LPVOID params);

	void ReceiveRobbyPacket();
	void ReceiveGameData();
};
