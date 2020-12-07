#include "pch.h"
#include "NetworkCommunicator.h"
#include "../packets.h"
#include "GameFramework.h"

#define SERVERPORT 9000

int recvn(SOCKET s, char* buf, int len, int flags) {
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

// 예외처리 어떻게 할 지 고민해보기
void NetworkCommunicator::Initialize(GameFramework* framework)
{
	m_Framework = framework;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0);

	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == m_Socket);
}

DWORD WINAPI NetworkCommunicator::ReceiveProc(LPVOID params)
{	
	NetworkCommunicator* communicator = (NetworkCommunicator*)params;

	OutputDebugString(L"ReceiveProc\n");

	// 이것보다는 씬을 전환할 때 인터럽트를 걸 수 있으면 좋겠다.
	SceneManager& scene = communicator->m_Framework->m_SceneManager;

	communicator->m_CurScene = SceneID::LOBBY;

	while (true) {
		switch (communicator->m_CurScene) {
		case SceneID::LOBBY:
			OutputDebugStringA("Lobby Scene");
			communicator->ReceiveRobbyPacket();
			communicator->m_CurScene = SceneID::GAME;
			break;
		case SceneID::GAME:
			communicator->ReceiveGameData();
			communicator->m_CurScene = SceneID::RESULT;
		case SceneID::RESULT: {
			result_packet::TimeOver packet;
			recvn(communicator->m_Socket, (char*)&packet, sizeof(packet), 0);

			 communicator->m_Framework->m_SceneManager.ChangeScene(SceneID::LOBBY );
			 communicator->m_CurScene = SceneID::LOBBY;
			OutputDebugStringA("Result Scene");
			break;
		}
		default:
			break;
		}
	}
}

bool NetworkCommunicator::Connect(const char* ip_addr, const String& nickname)
{
	// 1. 서버와 연결
    SOCKADDR_IN server_addr{ };
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_addr);
    server_addr.sin_port = htons(SERVERPORT);

	int rtv = connect(m_Socket, (SOCKADDR*)&server_addr, sizeof(server_addr));
	if (SOCKET_ERROR == rtv) {
		OutputDebugString(L"connect 실패\n");
		return false;
	}

	// 2. nickname 보내기
	lobby_packet::Nickname packet;
	packet.id = 0;
	ZeroMemory(packet.name, sizeof(TCHAR) * 16);
	memcpy(packet.name, nickname.c_str(), nickname.size() * sizeof(TCHAR));
	send(m_Socket, (const char*)&packet, sizeof(packet), 0);

	// 3. ReceiveThread 돌리기
	CreateThread(NULL, 0, ReceiveProc, (LPVOID)this, NULL, NULL);
	return true;
}

void NetworkCommunicator::SendChatting(const String& chatting)
{
	using namespace lobby_packet;
	Chatting packet;
	packet.type = PacketType::CHATING;
	packet.size = chatting.length() * sizeof(wchar_t);
	for (int i = 0; i < chatting.length(); ++i)
		packet.string[i] = chatting[i];
	send(m_Socket, (char*)&packet, sizeof(packet), 0);	// 문자열 개수만큼만 보내기
}

void NetworkCommunicator::SendReady()
{
	lobby_packet::Ready packet;
	packet.type = lobby_packet::PacketType::READY;
	packet.size = 0;
	
	send(m_Socket, (const char*)&packet, sizeof(packet), 0);
	OutputDebugString(TEXT("ready 보냄\n"));
}

void NetworkCommunicator::SendPlayerState(PlayerState state)
{
	game_packet::CS_PlayerState packet;
	packet.type = game_packet::PacketType::PlayerState;
	packet.state = state;
	send(m_Socket, (char*)&packet, sizeof(packet), 0);	// 문자열 개수만큼만 보내기
}

void NetworkCommunicator::SendBomb()
{
	game_packet::CS_Bomb packet;
	packet.type = game_packet::PacketType::Bomb;

	send(m_Socket, (char*)&packet, sizeof(packet), 0);
}

struct LobbyHeader {
#pragma pack(1)
	lobby_packet::PacketType type;
	unsigned short size;
#pragma pack(1)
};

void NetworkCommunicator::ReceiveRobbyPacket()
{
	using namespace lobby_packet;
	LobbyHeader header;

	while (true) {
		recvn(m_Socket, (char*)&header, sizeof(header), 0);

		switch (header.type) {
		case PacketType::LOBBY_INFO: { // 누군가 로비에 입장했을 때 이 패킷을 받는다
			LobbyInfo packet;
			recvn(m_Socket, (char*)&packet.users, sizeof(Nickname) * 4, 0);

			m_Framework->m_Objects.SetNicknames(packet.users);
			break;
		}
		case PacketType::READY: {
			int player_id = header.size;
			m_Framework->m_Objects.SetReady(player_id);
			break;
		}
		case PacketType::CHATING: {
			TCHAR buf[256];
			recvn(m_Socket, (char*)buf, header.size, 0);
			m_Framework->m_Objects.AddChatting(buf);
			break;
		}
		case PacketType::GAME_START: {
			SC_GameStart packet;
			packet.type = PacketType::GAME_START;
			packet.size = 0;
			send(m_Socket, (const char*)&packet, sizeof(packet), 0);

			m_Framework->m_SceneManager.ChangeScene(SceneID::GAME);
			OutputDebugStringA("게임씬 시작~!");
			return;
		}
		}
	}
}

struct WorldHeader {
	unsigned short player_count;
	unsigned short bomb_count;
	unsigned short explosive_count;
};

void NetworkCommunicator::ReceiveGameData()
{
	using namespace game_packet;
	SC_WorldState packet;

	while (true) {
		// 패킷 받기
		recvn(m_Socket, (char*)&packet, sizeof(packet), 0);

		if (1 >= packet.player_count) {
			game_packet::CS_GameOver gameover;
			gameover.type = game_packet::PacketType::GameOver;
			send(m_Socket, (const char*)&gameover, sizeof(gameover), 0);

			m_Framework->m_SceneManager.ChangeScene(SceneID::RESULT);
			return;
		}

		PlayerInfo player_info;
		char* player_start = packet.buf 
			+ packet.explosive_count * sizeof(SendExplosiveInfo)
			+ packet.bomb_count * sizeof(SendBombInfo);
		for (int i = 0; i < packet.player_count; ++i) {
			memcpy(&player_info, player_start, sizeof(PlayerInfo));
			player_start += sizeof(PlayerInfo);

			if (PlayerState::DEAD == player_info.state) {
				m_Framework->m_Objects.ranking.push_front(player_info.id);

			}
		}

		m_Framework->m_Objects.SetWorldState(packet);
	}
}
