#include "pch.h"
#include "NetworkCommunicator.h"
#include "../packets.h"

#include "GameFramework.h"

#define SERVERPORT 9000

DWORD __stdcall NetworkCommunicator::ServerMain(LPVOID network_communicator)
{
	NetworkCommunicator* communicator = (NetworkCommunicator*)network_communicator;
	
	communicator->Initialize();
	OutputDebugString(L"Server Main\n");

	while (true) {
		if (communicator->m_MessageQueue.empty()) continue;
		NETWORK_MASSAGE msg =  communicator->m_MessageQueue.front();
		communicator->TranselateMessage(msg);
		communicator->m_MessageQueue.pop();
	}
}

void NetworkCommunicator::PushMessage(NETWORK_MASSAGE msg)
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

void NetworkCommunicator::TranselateMessage(NETWORK_MASSAGE msg)
{
	switch (msg) {
	case NETWORK_MASSAGE::CONNECT:
		OutputDebugString(L"test\n");
		this->Connect("192.168.43.216", TEXT("혜리무"));
		break;
	case NETWORK_MASSAGE::PLAYER_UP:
		this->SendPlayerState(PlayerState::UP);
		break;
	case NETWORK_MASSAGE::PLAYER_DOWN:
		this->SendPlayerState(PlayerState::DOWN);
		break;
	case NETWORK_MASSAGE::PLAYER_LEFT:
		this->SendPlayerState(PlayerState::LEFT);
		break;
	case NETWORK_MASSAGE::PLAYER_RIGHT:
		this->SendPlayerState(PlayerState::RIGHT);
		break;
	case NETWORK_MASSAGE::PLAYER_IDLE:
		this->SendPlayerState(PlayerState::IDLE);
		break;
	case NETWORK_MASSAGE::UPDATE:
		this->ReceiveWorldData();
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

void NetworkCommunicator::ReceiveRobbyPacket()
{
}

struct WorldHeader {
	unsigned short player_count;
	unsigned short bomb_count;
	unsigned short explosive_count;
};

void NetworkCommunicator::ReceiveWorldData()
{
	PlayerInfo p[4];

	WorldHeader world_header;

	while (true) {
		int rtv = recv(m_Socket, (char*)&world_header, sizeof(world_header), 0);

		if (SOCKET_ERROR == rtv) {
			OutputDebugString(L"recv 실패\n");
		}


		for (int i = 0; i < world_header.player_count; ++i) {
			recv(m_Socket, (char*)&p[i], sizeof(PlayerInfo), 0);
		}

		OutputDebugStringA(std::to_string(p[0].pos.r).c_str());
		OutputDebugStringA("\n");

		m_Framework->m_SceneManager.UpdateCurrentScene(p[0]);
	}
}
