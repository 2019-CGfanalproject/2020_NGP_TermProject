#include "pch.h"
#include <chrono>


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

PlayerInfo playerinfo[4];

BombInfo bomb[12];

int number_of_clients;
int ReadyCount;
int alivePlayer;

game_packet::CS_PlayerState PlayerPacket[4];
game_packet::CS_Bomb BombPacket;
game_packet::SC_WorldState WorldPacket;

float vel_x[4], vel_y[4];



struct LobbyPacketHeader {
#pragma pack(1)
	lobby_packet::PacketType type;
	unsigned short size;
#pragma pack()
};

struct GamePacketHeader {
#pragma pack(1)
	game_packet::PacketType type;

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
	SceneCheck = SceneID::GAME;
	HANDLE hThread;
	while (true) {

		switch (SceneCheck) {

		case SceneID::LOBBY:

			LobbyCummunicate(arg);


			break;

		case SceneID::GAME:
			
			playerinfo[0].pos.r = 1;
			playerinfo[0].pos.c = 1;

			playerinfo[1].pos.r = 8;
			playerinfo[1].pos.c = 1;

			playerinfo[2].pos.r = 1;
			playerinfo[2].pos.c = 8;

			playerinfo[3].pos.r = 8;
			playerinfo[3].pos.c = 8;
			
			
			GameCommunicate(arg);
			break;

		case SceneID::RESULT:

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
	
	LobbyPacketHeader header;
	
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
	SceneCheck = SceneID::GAME;

}


void GameCommunicate(LPVOID arg) {
	ClientInfo* client = (ClientInfo*)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	addrlen = sizeof(clientaddr);
	getpeername(client->client, (SOCKADDR*)&clientaddr, &addrlen);
	alivePlayer = number_of_clients;
	PlayerState playerstate;

	GamePacketHeader Gameheader;

	while (true) {
		// 너는 리시브만 받고 업데이트로(전역 변수 업데이트 해줘)
		
		retval = recvn(client->client, (char*)&Gameheader, sizeof(Gameheader), 0);
		if (retval == SOCKET_ERROR) {
			error_display("recv");
		}
		//cout << " packet type:"<<(int)Gameheader.type << '\n' ;

		if (Gameheader.type == game_packet::PacketType::PlayerState) {
			retval = recvn(client->client, (char*)&PlayerPacket[client->index].state, sizeof(PlayerPacket[client->index].state), 0);
			//cout <<"player state: " <<(int)PlayerPacket[client->index].state << '\n';
		}

		if (Gameheader.type == game_packet::PacketType::Bomb) {
			// bomb의 위치는 플레이어의 위치
			if (playerinfo[client->index].bomb_count != 0) {
				bomb[(client->index*3) + playerinfo[client->index].bomb_count].pos.r=playerinfo[client->index].pos.r;
				bomb[(client->index*3) + playerinfo[client->index].bomb_count].pos.c=playerinfo[client->index].pos.c;
				--playerinfo[client->index].bomb_count;
				
			}

		}
		// 충돌 처리

	}


}


void ResultCommunicate(LPVOID arg) {

}

// 클라에서 사용하는 전역 변수 이용해서 업데이트하고 그걸 전부다 한테 send
unsigned __stdcall UpdateAndSend(LPVOID arg) {

	

	while (SceneCheck == SceneID::GAME) {

		auto  t0 = chrono::high_resolution_clock::now();

		for (int i = 0; i < number_of_clients; ++i) {
			switch (PlayerPacket[i].state) {
			case PlayerState::IDLE:
				vel_x[i] = 0;
				vel_y[i] = 0;
				break;
			case PlayerState::UP:
				vel_y[i] = 1;
				break;
			case PlayerState::DOWN:
				vel_y[i] = -1;
				break;
			case PlayerState::LEFT:
				vel_x[i] = -1;
				break;
			case PlayerState::RIGHT:
				vel_x[i] = 1;
				break;
				
			}
			float t = 1.f / 30.f;
			playerinfo[i].pos.r = playerinfo[i].pos.r+ vel_x[i] * t;
			playerinfo[i].pos.c = playerinfo[i].pos.c+ vel_y[i] * t;

			
		}

		// 패킷 보내기전 
		WorldPacket.player_count = alivePlayer;
		WorldPacket.bomb_count = 0;
		WorldPacket.explosive_count = 0;

		for (int i = 0; i < number_of_clients; ++i) {
			memcpy(WorldPacket.buf + i* sizeof(playerinfo), &playerinfo[i], sizeof(playerinfo));
		}


		auto t1 = chrono::high_resolution_clock::now();
		auto elpased = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
		// 프레임 30분의 1 
		while (elpased < 32) {

			elpased = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - t0).count();
			
		}

		for (int i = 0; i < number_of_clients; ++i) {
			send(clients[i].client, (char*)&WorldPacket, sizeof(WorldPacket), 0);
			
			cout << "플레이어 수:" << (int)WorldPacket.player_count << '\n'
				<< "플레이어 정보c" << (int)playerinfo->pos.c << '\n'
				<< "플레이어 정보r" << (int)playerinfo->pos.r << '\n';
		}

		//if (alivePlayer == 0) { // 원래는 1
		//	SceneCheck = SceneID::RESULT;
		//	break;
		//}
	}
	return 0;
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
	HANDLE hThread2;

	//데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;

	while (1) {
		//accept()
		
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			error_display((char*)"accept()");
			break;
		}
		cout << "accept\n";
	
		// 초기화
		clients[number_of_clients].client = client_sock;
		clients[number_of_clients].index = number_of_clients;
		SceneCheck = SceneID::LOGIN;

		hThread = (HANDLE)_beginthreadex(NULL, 0, ClinetsThread, (LPVOID)&clients[number_of_clients], 0, NULL);

		++number_of_clients;
		
		hThread2 = (HANDLE)_beginthreadex(NULL, 0, UpdateAndSend, 0, 0, NULL);
			
		if (hThread == NULL) { closesocket(client_sock); }
		else {
			CloseHandle(hThread);
		}
		if (hThread2 != NULL) {
			CloseHandle(hThread2);
		}

	}



	closesocket(client_sock);
	closesocket(listen_sock);

	WSACleanup();

}