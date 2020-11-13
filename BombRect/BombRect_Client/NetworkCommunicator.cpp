#include "pch.h"
#include "NetworkCommunicator.h"
#include "packets.h"

#define SERVERPORT 9000

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
	if (SOCKET_ERROR == rtv);

	login_packet::CS_Nickname packet{ };
	
	for (int i = 0; i < nickname.length(); ++i)
		packet.buf[i] = nickname[i];

	send(m_Socket, (const char*)&packet, sizeof(packet), 0);
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

void NetworkCommunicator::SendPlayerState()
{
}

void NetworkCommunicator::SendBomb()
{
}

void NetworkCommunicator::ReceiveRobbyPacket()
{
}

DWORD __stdcall NetworkCommunicator::ReceiveWorldData(LPVOID params)
{
	return 0;
}
