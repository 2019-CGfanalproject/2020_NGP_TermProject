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

// ����ó�� ��� �� �� ����غ���
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

	// �̰ͺ��ٴ� ���� ��ȯ�� �� ���ͷ�Ʈ�� �� �� ������ ���ڴ�.
	SceneManager& scene = communicator->m_Framework->m_SceneManager;

	communicator->m_CurScene = SceneID::LOBBY;

	while (true) {
		switch (communicator->m_CurScene) {
		case SceneID::LOBBY:
			communicator->ReceiveRobbyPacket();
			communicator->m_CurScene = SceneID::GAME;
			break;
		case SceneID::GAME:
			communicator->ReceiveGameData();
			communicator->m_CurScene = SceneID::RESULT;
		case SceneID::RESULT:
			break;
		default:
			break;
		}
	}
}

bool NetworkCommunicator::Connect(const char* ip_addr, const String& nickname)
{
	// 1. ������ ����
    SOCKADDR_IN server_addr{ };
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_addr);
    server_addr.sin_port = htons(SERVERPORT);

	int rtv = connect(m_Socket, (SOCKADDR*)&server_addr, sizeof(server_addr));
	if (SOCKET_ERROR == rtv) {
		OutputDebugString(L"connect ����\n");
		return false;
	}

	// 2. nickname ������
	

	// 3. ReceiveThread ������
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
	send(m_Socket, (char*)&packet, sizeof(packet), 0);	// ���ڿ� ������ŭ�� ������
}

void NetworkCommunicator::SendReady()
{
	lobby_packet::Ready packet;
	packet.type = lobby_packet::PacketType::READY;
	packet.size = 0;
	
	send(m_Socket, (const char*)&packet, sizeof(packet), 0);
	OutputDebugString(TEXT("ready ����\n"));
}

void NetworkCommunicator::SendPlayerState(PlayerState state)
{
	game_packet::CS_PlayerState packet;
	packet.type = game_packet::PacketType::PlayerState;
	packet.state = state;
	send(m_Socket, (char*)&packet, sizeof(packet), 0);	// ���ڿ� ������ŭ�� ������
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
		case PacketType::READY: {
			OutputDebugStringA("ready ����");
			break;
		}
		case PacketType::CHATING: {
			// �� ����
			break;
		}
		case PacketType::GAME_START: {
			m_Framework->m_SceneManager.ChangeScene(SceneID::GAME);
			OutputDebugStringA("���Ӿ� ����~!");
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
	PlayerInfo p[4];
	SendBombInfo b[12];
	TilePos e[100];

	game_packet::SC_WorldState packet;

	while (true) {

		// ��Ŷ �ޱ�
		recvn(m_Socket, (char*)&packet, sizeof(packet), 0);

		// OutputDebugString((LPCWSTR)std::to_wstring(packet.player_count).c_str());
		if (packet.player_count != 1) {
			OutputDebugString(L"�̻��� ��;\n");
		}

		// ��Ŷ �ؼ�
		char* ptr = packet.buf;
	
		m_Framework->m_Objects.lock.lock();
		for (int i = 0; i < packet.player_count; ++i) {
			memcpy(&p[i], ptr, sizeof(PlayerInfo));
			ptr += sizeof(PlayerInfo);
		}
		m_Framework->m_Objects.lock.unlock();

		for (int i = 0; i < packet.bomb_count; ++i) {
			memcpy(&b[i], ptr, sizeof(SendBombInfo));
			ptr += sizeof(SendBombInfo);
		}

		for (int i = 0; i < packet.explosive_count; ++i) {
			memcpy(&e[i], ptr, sizeof(TilePos));
			ptr += sizeof(TilePos);

			OutputDebugStringA(std::to_string(e[i].r).c_str());
			OutputDebugStringA(", ");
			OutputDebugStringA(std::to_string(e[i].c).c_str());
			OutputDebugStringA("\n");
		}


		m_Framework->m_SceneManager.UpdateCurrentScene(p[0]);
		
		m_Framework->m_Objects.lock.lock();
		for (int i = 0; i < packet.explosive_count; ++i) {
			memcpy(&m_Framework->m_Objects.explo_info[i], 
				&e[i], sizeof(TilePos));
		}
		m_Framework->m_Objects.lock.unlock();
	}
}
