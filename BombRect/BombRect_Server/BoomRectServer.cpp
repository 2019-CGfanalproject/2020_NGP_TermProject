#include "pch.h"


const TilePos CLOSED_TILE_POS[] = {
   {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0},  {8, 0},
   {0, 1},															{8, 1},
   {0, 2},          {2, 2},         {4, 2},         {6, 2},         {8, 2},
   {0, 3},															{8, 3},
   {0, 4},          {2, 4},         {4, 4},         {6, 4},         {8, 4},
   {0, 5},															{8, 5},
   {0, 6},          {2, 6},         {4, 6},         {6, 6},         {8, 6},
   {0, 7},															{8, 7},
   {0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 8}, {7, 8},  {8, 8},
};

const bool ClosedTiles[9][9] = {
	{true,	true,	 true,	  true,	  true,	  true,	  true,	  true,	  true},
	{true,	false,   false,   false , false,  false,  false,  false , true},
	{true,	false,   true,    false , true,   false,  true,   false , true},
	{true,	false,   false,   false , false,  false,  false,  false , true},
	{true,	false,   true,    false , true,   false,  true,   false , true},
	{true,	false,   false,   false , false,  false,  false,  false , true},
	{true,	false,   true,    false , true,   false,  true,   false , true},
	{true,	false,   false,   false , false,  false,  false,  false , true},
	{true,	true,	 true,	  true,	  true,	  true,	  true,	  true,	  true}
};

SceneID SceneCheck;

struct ClientInfo {
	SOCKET client;
	int		index;
};
HANDLE hThread2;


ClientInfo clients[4];

PlayerInfo playerinfo[4];


struct S_Bombinfo {
	SendBombInfo bombinfo;
	int id;

	S_Bombinfo(SendBombInfo i_bombinfo,int i_id) {
		bombinfo.pos.r = i_bombinfo.pos.r;
		bombinfo.pos.c = i_bombinfo.pos.c;
		bombinfo.bomb_count_down = i_bombinfo.bomb_count_down;
		id = i_id;

	}
};

list <SendExplosiveInfo> explosions;

list<S_Bombinfo> bombs;

int number_of_clients;
int ReadyCount;
int alivePlayer;
unsigned int CountDown{ 0 };

game_packet::CS_PlayerState volatile PlayerPacket[4];
game_packet::CS_Bomb BombPacket;

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

void error_display(const char* msg) {
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
	HANDLE hThread;
	while (true) {

		switch (SceneCheck) {

		case SceneID::LOBBY:

			LobbyCummunicate(arg);


			break;

		case SceneID::GAME:
			
			
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
	
	// �г��� �޴� �κ�

	/*	retval = recvn(client->client, (char*)nicknamePacket.buf, sizeof(nicknamePacket.buf), 0);
		for (int i = 0; i < number_of_clients; ++i) {
			send(clients[i].client, (const char*)&nicknamePacket, sizeof(nicknamePacket), 0);
		}
	*/
	LobbyPacketHeader header;
	
	// ���� ��Ŷ�̶� ä�� ����;
	while (true) {
		retval = recvn(client->client, (char*)&header, sizeof(header), 0);

		//����
		if (header.type == lobby_packet::PacketType::READY) {
			cout << "Ready ����\n";
			lobby_packet::Ready readyPacket{};
			readyPacket.type = lobby_packet::PacketType::READY;

			readyPacket.size = client->index;
			
			if (!ReadyPressed) {
				cout << "����\n";
				++ReadyCount;
				ReadyPressed = true;
			}
			else {
				cout << "���� ���\n";
				--ReadyCount;
				ReadyPressed = false;

			}

			for (int i = 0; i < number_of_clients; ++i) {
				//���� ���� ��Ŷ ������
				send(clients[i].client, (char*)&readyPacket , sizeof(readyPacket), 0);

			}
			if (number_of_clients == ReadyCount) {
				// ���� ���� ��Ŷ ������

				lobby_packet::SC_GameStart startPacket{};
				startPacket.type = lobby_packet::PacketType::GAME_START;
				for (int i = 0; i < number_of_clients; ++i) {
					send(clients[i].client, (char*)&startPacket, sizeof(startPacket), 0);
					cout << "strat packet send\n";
				}
				break;
			}
		}

		//ä��
		if (header.type == lobby_packet::PacketType::CHATING) {
			//ä�� ��Ŷ ������

			char ChattingBuf[256]{};
			lobby_packet::Chatting chattingPacket;
			chattingPacket.type = lobby_packet::PacketType::CHATING;
			
			
			retval = recvn(client->client, (char*)chattingPacket.string, header.size, 0);
			for (int i = 0; i < number_of_clients; ++i) {
				send(clients[i].client, (char*)&chattingPacket, sizeof(chattingPacket), 0);
			}

		}
	}

	
	SceneCheck = SceneID::GAME;
	ResumeThread(hThread2);

}

PlayerState testState;
std::mutex g_m;

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
		// �ʴ� ���ú길 �ް� ������Ʈ��(���� ���� ������Ʈ ����)
		
		retval = recvn(client->client, (char*)&Gameheader, sizeof(Gameheader), 0);
		if (retval == SOCKET_ERROR) {
			error_display("recv");
		}
		cout << " packet type:"<<(int)Gameheader.type << '\n' ;

		if (Gameheader.type == game_packet::PacketType::PlayerState) {
			retval = recvn(client->client, (char*)&PlayerPacket[client->index].state, sizeof(PlayerPacket[client->index].state), 0);
			if (retval == SOCKET_ERROR) {
				error_display("recv");
			}
			cout <<"player state: " <<(int)PlayerPacket[client->index].state << '\n';
		}

		if (Gameheader.type == game_packet::PacketType::Bomb) {
			cout << "bomb ����\n";
			// bomb�� ��ġ�� �÷��̾��� ��ġ
			if (playerinfo[client->index].bomb_count != 0) {
				SendBombInfo bomb_tmp;
				bomb_tmp.bomb_count_down = 0;
				bomb_tmp.pos.r = std ::round(playerinfo[client->index].pos.r);
				bomb_tmp.pos.c = std::round(playerinfo[client->index].pos.c);
				bombs.emplace_back(bomb_tmp, (int)client->index);
			
			

			}
			
		}
		// �浹 ó��

	}


}


void ResultCommunicate(LPVOID arg) {

}

// Ŭ�󿡼� ����ϴ� ���� ���� �̿��ؼ� ������Ʈ�ϰ� �װ� ���δ� ���� send
unsigned __stdcall UpdateAndSend(LPVOID arg) {

	cout << "UpadteAndSend ����\n";
	//1��
	playerinfo[0].pos.r = 1.f;
	playerinfo[0].pos.c = 1.f;
	//2��
	playerinfo[1].pos.r = 8.f;
	playerinfo[1].pos.c = 1.f;
	////3��
	playerinfo[2].pos.r = 1.f;
	playerinfo[2].pos.c = 8.f;
	////4��
	playerinfo[3].pos.r = 8.f;
	playerinfo[3].pos.c = 8.f;

	while (true) {

		// �̵� ����
		for (int i = 0; i < number_of_clients; ++i) {

			switch (PlayerPacket[i].state) {
			case PlayerState::IDLE:
				vel_x[i] = 0;
				vel_y[i] = 0;
				break;
			case PlayerState::UP:
				vel_x[i] = 0;
				vel_y[i] = -1;
				break;
			case PlayerState::DOWN:
				vel_x[i] = 0;
				vel_y[i] = +1;
				break;
			case PlayerState::LEFT:
				vel_x[i] = -1;
				vel_y[i] = 0;

				break;
			case PlayerState::RIGHT:
				vel_x[i] = 1;
				vel_y[i] = 0;

				break;

			}
			float t = 1.f / 50.f;
			playerinfo[i].pos.r = playerinfo[i].pos.r + vel_x[i] * t;
			playerinfo[i].pos.c = playerinfo[i].pos.c + vel_y[i] * t;
		}

		//	// ��ź ���� �� ����
		//	
			//if (CountDown % 31 == 0) { // 1�� ���� ī��Ʈ ����
		S_Bombinfo* zero_bomb = nullptr;
		for (auto& bomb : bombs) {
			//bomb.bombinfo.bomb_count_down--;

			if (bomb.bombinfo.bomb_count_down == 0) {
				zero_bomb = &bomb;
				break;
			}

			// ����

		}

		if (zero_bomb) {

			// y�� ���� x�� ������ ���� Ȯ��
			for (int i = zero_bomb->bombinfo.pos.r; i < 9; ++i) {
				if (!ClosedTiles[i][(int)zero_bomb->bombinfo.pos.c]) {
					TilePos eptmp;
					eptmp.r = i;
					eptmp.c = zero_bomb->bombinfo.pos.c;
					explosions.emplace_back(eptmp);
				}
				else break;
			}
			for (int i = zero_bomb->bombinfo.pos.r; i > 0; --i) {
				if (!ClosedTiles[i][(int)zero_bomb->bombinfo.pos.c]) {
					TilePos eptmp;
					eptmp.r = i;
					eptmp.c = zero_bomb->bombinfo.pos.c;
					explosions.emplace_back(eptmp);
				}
				else break;
			}
			//x�� ���� y�� ������ ���� Ȯ��
			for (int i = zero_bomb->bombinfo.pos.c; i < 9; ++i) {
				if (!ClosedTiles[(int)zero_bomb->bombinfo.pos.r][i]) {
					TilePos eptmp;
					eptmp.r = zero_bomb->bombinfo.pos.r;
					eptmp.c = i;
					explosions.emplace_back(eptmp);
				}
				else break;
			}
			for (int i = zero_bomb->bombinfo.pos.c; i >= 0; --i) {
				if (!ClosedTiles[(int)zero_bomb->bombinfo.pos.r][i]) {
					TilePos eptmp;
					eptmp.r = zero_bomb->bombinfo.pos.r;
					eptmp.c = i;
					explosions.emplace_back(eptmp);
				}
				else break;
			}


		}

		game_packet::SC_WorldState WorldPacket{};
		// ��Ŷ �������� 
		WorldPacket.player_count = alivePlayer;
		WorldPacket.bomb_count = bombs.size();
		WorldPacket.explosive_count = explosions.size();

		char* cur_ptr = WorldPacket.buf;
		//�÷��̾� ���� ����
		for (int i = 0; i < number_of_clients; ++i) {
			memcpy(cur_ptr, &playerinfo[i], sizeof(PlayerInfo));
			cur_ptr += sizeof(PlayerInfo);
		}

		//��ź ���� ����
		for (auto& bomb : bombs) {
			memcpy(cur_ptr, &bomb.bombinfo, sizeof(SendBombInfo));
			cur_ptr += sizeof(SendBombInfo);
		}

		//���� ���� ����
		for (auto& explosion : explosions) {
			memcpy(cur_ptr, &explosion, sizeof(SendExplosiveInfo));
			cur_ptr += sizeof(SendExplosiveInfo);
		}

		for (int i = 0; i < number_of_clients; ++i) {
			int retval = send(clients[i].client, (char*)&WorldPacket, sizeof(WorldPacket), 0);
			if (retval == SOCKET_ERROR) {
				error_display("send");
			}
		}

		bombs.clear();
		::Sleep(32);

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
	
	}
	HANDLE hThread;
	
	//������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	bool acceptflag = false;
	while (1) {
		//accept()
		
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			error_display((char*)"accept()");
			break;
		}
		cout << "accept\n";
	
		// �ʱ�ȭ
		clients[number_of_clients].client = client_sock;
		clients[number_of_clients].index = number_of_clients;
		SceneCheck = SceneID::LOGIN;

		hThread = (HANDLE)_beginthreadex(NULL, 0, ClinetsThread, (LPVOID)&clients[number_of_clients], 0, NULL);

		++number_of_clients;
		if (!acceptflag) {
			cout << "������Ʈ ����\n";
			hThread2 = (HANDLE)_beginthreadex(NULL, 0, UpdateAndSend, 0,CREATE_SUSPENDED, NULL);
			acceptflag = true;
		}
		if (hThread == NULL) { closesocket(client_sock); }
		else {
			CloseHandle(hThread);
		}
		

	}



	closesocket(client_sock);
	closesocket(listen_sock);

	WSACleanup();

}