#include "S_global.h"
#include "../packets.h"

//void error_quit(char* msg)
//{
//	LPVOID lpMsgBuf;
//	FormatMessage(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//		NULL,
//		WSAGetLastError(),
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPSTR)&lpMsgBuf, 0, NULL);
//	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
//	LocalFree(lpMsgBuf);
//	exit(1);
//
//}

SceneID SceneCheck;

struct ClientInfo {
	SOCKET client;
	int		index;
};
ClientInfo clients[4];


int number_of_clients;
int ReadyCount;

struct PacketHeader {
#pragma pack(1)
	lobby_packet::PacketType type;
	unsigned short size;
#pragma pack()
};


//lobby_info{
//	{0,nullptr},{0,nullptr}
//	{0,nullptr},{0,nullptr}
//}

void error_display(char* msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}



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


unsigned __stdcall ClinetsThread(LPVOID arg) {
	SceneCheck = SceneID::LOBBY;
	
	while (true) {

		switch (SceneCheck) {

		case SceneID::LOBBY:

			LobbyCummunicate(arg);


			break;


		}
	}
	number_of_clients--;

}


void LobbyCummunicate(LPVOID arg)
{
	ClientInfo* client = (ClientInfo *)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	addrlen = sizeof(clientaddr);
	getpeername(client->client, (SOCKADDR*)&clientaddr, &addrlen);
	bool ReadyPressed = false; 
	lobby_packet::CS_Nickname nicknamePacket{};
	
		retval = recvn(client->client, (char*)nicknamePacket.buf, sizeof(nicknamePacket.buf), 0);
		for (int i = 0; i < number_of_clients; ++i) {
			send(clients[i].client, (const char*)&nicknamePacket, sizeof(nicknamePacket), 0);
		}
	
	PacketHeader header;
	
	// 레디 패킷이랑 채팅 구분;
	while (true) {
		retval = recvn(client->client, (char*)&header, sizeof(header), 0);

		//레디
		if (header.type == lobby_packet::PacketType::READY) {
			lobby_packet::Ready readyPacket{};
			readyPacket.type = lobby_packet::PacketType::READY;

			readyPacket.size = client->index;
			
			if (!ReadyPressed) {
				++ReadyCount;
				ReadyPressed = true;
			}
			else {
				--ReadyCount;
			}

			for (int i = 0; i < number_of_clients; ++i) {
				//레디 정보 패킷 보내기
				send(clients[i].client, (char*)&readyPacket , sizeof(readyPacket), 0);

			}
			if (number_of_clients == ReadyCount) {
				// 게임 시작 패킷 보내기
				lobby_packet::SC_GameStart startPacket;
				startPacket.type = lobby_packet::PacketType::GAME_START;
				for (int i = 0; i < number_of_clients; ++i) {
					send(clients[i].client, (char*)&startPacket, sizeof(startPacket), 0);
				}

				break;
			}


		}
		//채팅
		if (header.type == lobby_packet::PacketType::CHATING) {
			//채팅 패킷 보내기
			char ChattingBuf[256]{};
			lobby_packet::Chatting chattingPacket;
			chattingPacket.type = lobby_packet::PacketType::CHATING;
			
			
			retval = recvn(client->client, (char*)chattingPacket.string, header.size, 0);
			for (int i = 0; i < number_of_clients; ++i) {
				send(clients[i].client, (char*)&chattingPacket, sizeof(chattingPacket), 0);
			}

		}
	}

	
	closesocket(client->client);


}


void GameCommunicate(LPVOID arg) {

}


int main(int argc, char* argv[])
{
	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	//socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) {
		cout << "socket()\n";
	}// error_quit("socket()");

	//bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		cout << "bind()\n";
	}// error_quit("bind()");

	//listen()
	retval = listen(listen_sock, SOMAXCONN);

	cout << "listen\n";
	if (retval == SOCKET_ERROR) {
		cout << "listen()\n";
	} //error_quit("listen()");
	
	HANDLE hThread;
	
	//데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;

	while (1) {
		//accept()
		
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		cout << "accept\n";
		if (client_sock == INVALID_SOCKET) {
			error_display((char*)"accept()");
			break;
		}
		// 초기화
		clients[number_of_clients].client = client_sock;
		clients[number_of_clients].index = number_of_clients;
		SceneCheck = SceneID::LOGIN;

		hThread = (HANDLE)_beginthreadex(NULL, 0, ClinetsThread, (LPVOID)&clients[number_of_clients], 0, NULL);

		++number_of_clients;
	
			
		if (hThread == NULL) { closesocket(client_sock); }
		else {
			CloseHandle(hThread);
		}

	}



	closesocket(client_sock);
	closesocket(listen_sock);

	WSACleanup();

}