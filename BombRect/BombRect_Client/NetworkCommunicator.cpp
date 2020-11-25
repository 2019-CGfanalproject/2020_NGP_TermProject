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

DWORD __stdcall NetworkCommunicator::ServerMain(LPVOID network_communicator)
{
	NetworkCommunicator* communicator = (NetworkCommunicator*)network_communicator;
	
	communicator->Initialize();
	OutputDebugString(L"Server Main\n");

	communicator->m_CurScene = SceneID::LOGIN;

	std::string loopback{ "127.0.0.1" };
	std::string shphone{ "192.168.43.216" };

	communicator->Connect(shphone.c_str(), TEXT("혜리무"));

	while (true) {
		switch (communicator->m_CurScene) {
		case SceneID::LOGIN:
			communicator->ReceiveRobbyPacket();
			break;
		case SceneID::GAME:
			communicator->ReceiveGameData();
			break;
		}
	}

	//while (true) {
	//	if (communicator->m_MessageQueue.empty()) continue;
	//	CommunicateMessage msg =  communicator->m_MessageQueue.front();
	//	communicator->TranselateMessage(msg);
	//	communicator->m_MessageQueue.pop();
	//}
}

// Main Thread에서만 호출해야 한다, Network Thread에서 부르면 queue 터짐
void NetworkCommunicator::PushMessage(CommunicateMessage msg)
{
	m_MessageQueue.push(msg);
}

void NetworkCommunicator::SetFramework(GameFramework* framework)
{
	m_Framework = framework;
}

// 예외처리 어떻게 할 지 고민해보기
void NetworkCommunicator::Initialize()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0);

	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == m_Socket);
}

void NetworkCommunicator::Connect(const char* ip_addr, const String& nickname)
{
    SOCKADDR_IN server_addr{ };
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_addr);
    server_addr.sin_port = htons(SERVERPORT);

	int rtv = connect(m_Socket, (SOCKADDR*)&server_addr, sizeof(server_addr));
	if (SOCKET_ERROR == rtv) {
		OutputDebugString(L"connect 실패\n");
	}

	//TCHAR buf[16] = { };
	//
	//for (int i = 0; i < nickname.length(); ++i)
	//	buf[i] = nickname[i];

	// send(m_Socket, (const char*)buf, sizeof(TCHAR) * 16, 0);
}

void NetworkCommunicator::TranselateMessage(CommunicateMessage msg)
{
	switch (msg) {
	case CommunicateMessage::CONNECT: {
		OutputDebugString(L"test\n");

		std::string loopback{ "127.0.0.1" };
		std::string shphone{ "192.168.43.216" };

		this->Connect(shphone.c_str(), TEXT("혜리무"));
		break;
	}
	case CommunicateMessage::PLAYER_UP:
		this->SendPlayerState(PlayerState::UP);
		break;
	case CommunicateMessage::PLAYER_DOWN:
		this->SendPlayerState(PlayerState::DOWN);
		break;
	case CommunicateMessage::PLAYER_LEFT:
		this->SendPlayerState(PlayerState::LEFT);
		break;
	case CommunicateMessage::PLAYER_RIGHT:
		this->SendPlayerState(PlayerState::RIGHT);
		break;
	case CommunicateMessage::PLAYER_IDLE:
		this->SendPlayerState(PlayerState::IDLE);
		break;
	case CommunicateMessage::UPDATE:
		this->ReceiveGameData();
		break;
	default:
		break;
	}
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
	// recv(m_Socket, )	 헤더를 먼저 받고


	// recv(m_Socket, )  이후 맞는 데이터를 받는다.
}

void NetworkCommunicator::SendReady()
{
	lobby_packet::Ready packet;
	packet.type = lobby_packet::PacketType::READY;
	packet.size = 0;
	
	send(m_Socket, (const char*)&packet, sizeof(packet), 0);
	OutputDebugStringA("ready 보냄");

	//recvn(m_Socket, (char*)&packet, sizeof(packet), 0);
	//OutputDebugStringA("ready 받음");

	//{
	//	lobby_packet::SC_GameStart packet;

	//	recvn(m_Socket, (char*)&packet, sizeof(packet), 0);
	//	OutputDebugStringA("game start 받음");
	//}
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
			OutputDebugStringA("ready 받음");
			break;
		}
		case PacketType::CHATING: {
			// 더 받음
			break;
		}
		case PacketType::GAME_START: {
			m_CurScene = SceneID::GAME;
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
	PlayerInfo p[4];

	game_packet::SC_WorldState packet;

	while (true) {
		recvn(m_Socket, (char*)&packet, sizeof(packet), 0);


		//memcpy(&p[0], packet.buf, sizeof(PlayerInfo));

		OutputDebugString((LPCWSTR)std::to_wstring(packet.player_count).c_str());
		if (packet.player_count != 1) {
			// OutputDebugString(L"이상한 값;\n");
		}
		for (int i = 0; i < packet.player_count; ++i) {
			memcpy(&p[i], packet.buf + (i * sizeof(PlayerInfo)), sizeof(PlayerInfo));
		}

		//int rtv = recv(m_Socket, (char*)&world_header, sizeof(world_header), 0);

		//if (SOCKET_ERROR == rtv) {
		//	OutputDebugString(L"recv 실패\n");
		//}

		//char buf[1024];

		//recv(m_Socket, (char*)buf, sizeof(buf), 0);

		//// 플레이어 카운트에 이상한 값이 들어온다.
		//for (int i = 0; i < world_header.player_count; ++i) {
		//	memcpy(&p[i], buf + i * sizeof(PlayerInfo), sizeof(PlayerInfo));
		//}


		// 월드 패킷의 1024바이트를 모두 보낸다;;
		// 그럼 여기서 무조건 1024바이트를 받은 다음 해석해야지 문제가 없다.
		// 이건 낭비 아니냐??


		OutputDebugStringA(std::to_string(p[0].pos.r).c_str());
		OutputDebugStringA("\n");

		m_Framework->m_SceneManager.UpdateCurrentScene(p[0]);
	}
}
