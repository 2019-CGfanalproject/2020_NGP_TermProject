#include "pch.h"

mutex g_lock;

#define BOMB 2

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

char ClosedTiles[9][9] = {
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


volatile ClientInfo clients[4];

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

struct S_ExplosiveInfo {
	SendExplosiveInfo explosiveinfo;
	unsigned int explosive_countdown ;
	S_ExplosiveInfo(SendExplosiveInfo explosiveinfo, unsigned int explosive_countdown) :explosiveinfo(explosiveinfo), explosive_countdown(explosive_countdown){}
};

list <S_ExplosiveInfo> explosions;

list<S_Bombinfo> bombs;

int number_of_clients;
int ReadyCount;
int alivePlayer;
result_packet::TimeOver timeoverPacket;
unsigned int CountDown{ 0 };

lobby_packet::LobbyInfo nicknamePacket{};

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

			ResultCommunicate(arg);
			break;

		}
	}

	number_of_clients--;

}

mutex bombLock;
void SetBomb(SendBombInfo bomb_tmp, ClientInfo* client) {

	for (auto bomb : bombs) {
		if (((bomb.bombinfo.pos.r == bomb_tmp.pos.r)
			&& (bomb.bombinfo.pos.c == bomb_tmp.pos.c))) {
			return;
		}
	}
	if (playerinfo[client->index].bomb_count > 0) {
		bombLock.lock();
		ClosedTiles[(int)bomb_tmp.pos.r][(int)bomb_tmp.pos.c] = BOMB;
		bombs.emplace_back(bomb_tmp, (int)client->index);
		bombLock.unlock();
		playerinfo[client->index].bomb_count--;
	}

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
	
	// �г��� �޴� �κ�

		retval = recvn(client->client, (char*)&nicknamePacket.users[client->index], sizeof(lobby_packet::Nickname), 0);
		nicknamePacket.users[client->index].id = client->index;
		nicknamePacket.type = lobby_packet::PacketType::LOBBY_INFO;
		nicknamePacket.size = 0;
		for (int i = 0; i < number_of_clients; ++i) {
			send(clients[i].client, (const char*)&nicknamePacket, sizeof(lobby_packet::LobbyInfo), 0);
		}
	
	LobbyPacketHeader header;
	
	// ���� ��Ŷ�̶� ä�� ����;
	while (true) {
		retval = recvn(client->client, (char*)&header, sizeof(header), 0);

		//����
		if (header.type == lobby_packet::PacketType::READY) {
			cout << "Ready ����\n";
			if (number_of_clients <= 1) continue;
			
			lobby_packet::Ready readyPacket{};
			readyPacket.type = lobby_packet::PacketType::READY;
			readyPacket.size = client->index;
			if (!ReadyPressed ) {
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
				for(int i =0; i < number_of_clients ;++i)
				send(clients[i].client, (char*)&startPacket, sizeof(startPacket), 0);	
				
				ResumeThread(hThread2);
			}
		}

		//���� ���� ��Ŷ
		if (header.type == lobby_packet::PacketType::GAME_START) {
			cout << client->index <<'\n';
			cout << "���� ��Ŷ ����\n";
			SceneCheck = SceneID::GAME;
			
			break;
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
		// �ʴ� ���ú길 �ް� ������Ʈ��(���� ���� ������Ʈ ����)

		retval = recvn(client->client, (char*)&Gameheader, sizeof(Gameheader), 0);
		if (retval == SOCKET_ERROR) {
			error_display("recv");
		}

		PlayerState player_state;

		if (Gameheader.type == game_packet::PacketType::PlayerState) {
			retval = recvn(client->client, (char*)&player_state, sizeof(PlayerPacket[client->index].state), 0);

			g_lock.lock();
			PlayerPacket[client->index].state = player_state;
			g_lock.unlock();

			if (retval == SOCKET_ERROR) {
				error_display("recv");
			}
			switch (PlayerPacket[client->index].state) {
			case PlayerState::IDLE:
				cout << "player state: �⺻ \n";
				break;
			case PlayerState::UP:
				cout << "player state:�� \n";
				break;
			case PlayerState::DOWN:
				cout << "player state:�� \n";
				break;
			case PlayerState::RIGHT:
				cout << "player state:�� \n";
				break;
			case PlayerState::LEFT:
				cout << "player state:�� \n";
				break;
			}
		}

		if (Gameheader.type == game_packet::PacketType::Bomb) {
			// bomb�� ��ġ�� �÷��̾��� ��ġ
			if (playerinfo[client->index].bomb_count > 0) {
				SendBombInfo bomb_tmp;
				bomb_tmp.bomb_count_down = 3000;
				bomb_tmp.pos.r = std::round(playerinfo[client->index].pos.r);
				bomb_tmp.pos.c = std::round(playerinfo[client->index].pos.c);


				SetBomb(bomb_tmp, client);

			}

		}

		if (Gameheader.type == game_packet::PacketType::GameOver) {

			SceneCheck = SceneID::RESULT;
			break;
		}


		
	}


}


void ResultCommunicate(LPVOID arg) {
	ClientInfo* client = (ClientInfo*)arg;
	timeoverPacket.time_over = true;
	Sleep(2000);
	send(clients[client->index].client, (const char*)&timeoverPacket, sizeof(result_packet::TimeOver), 0);
	SceneCheck = SceneID::LOBBY;
}


// Ŭ�󿡼� ����ϴ� ���� ���� �̿��ؼ� ������Ʈ�ϰ� �װ� ���δ� ���� send
unsigned __stdcall UpdateAndSend(LPVOID arg) {

	cout << "UpadteAndSend ����\n";
	//1��
	playerinfo[0].pos.r = 1.f;
	playerinfo[0].pos.c = 1.f;
	//2��
	playerinfo[1].pos.r = 7.f;
	playerinfo[1].pos.c = 1.f;
	////3��
	playerinfo[2].pos.r = 1.f;
	playerinfo[2].pos.c = 7.f;
	////4��
	playerinfo[3].pos.r = 7.f;
	playerinfo[3].pos.c = 7.f;


	while (true) {

		// �̵� �� �浹 ó��
		PlayerState player_state[4];

		for (int i = 0; i < number_of_clients; ++i) {
			if (playerinfo[i].state == PlayerState::DEAD)continue;
			g_lock.lock();
			player_state[i] = PlayerPacket[i].state;
			g_lock.unlock();

			switch (player_state[i]) {
			case PlayerState::IDLE:
				playerinfo[i].state = PlayerState::IDLE;
				vel_x[i] = 0;
				vel_y[i] = 0;
				break;
			case PlayerState::UP:
				playerinfo[i].state = PlayerState::UP;
				if ((1.2 < playerinfo[i].pos.r && playerinfo[i].pos.r < 2.8) ||
					(3.2 < playerinfo[i].pos.r && playerinfo[i].pos.r < 4.8) ||
					(5.2 < playerinfo[i].pos.r && playerinfo[i].pos.r < 6.8)) {
					vel_x[i] = 0;
					vel_y[i] = 0;
				}//  ��ź �浹 ó�� 0.2 -0.1;
				else if (ClosedTiles[(int)(playerinfo[i].pos.r + 0.4)][(int)(playerinfo[i].pos.c-0.1)] == BOMB) {
					vel_x[i] = 0;
					vel_y[i] = 0;

				}
				else {
					vel_x[i] = 0;
					vel_y[i] = -1;
					playerinfo[i].pos.r = std::round(playerinfo[i].pos.r);
				}
				break;
			case PlayerState::DOWN:
				playerinfo[i].state = PlayerState::DOWN;
				if ((1.2 < playerinfo[i].pos.r && playerinfo[i].pos.r < 2.8) ||
					(3.2 < playerinfo[i].pos.r && playerinfo[i].pos.r < 4.8) ||
					(5.2 < playerinfo[i].pos.r && playerinfo[i].pos.r < 6.8)) {
					vel_x[i] = 0;
					vel_y[i] = 0;
				}//  ��ź �浹 ó�� 0.99 +0.1;
				else if (ClosedTiles[(int)(playerinfo[i].pos.r)][(int)(playerinfo[i].pos.c + 1)] == BOMB) {
					vel_x[i] = 0;
					vel_y[i] = 0;

				}
				else {
					playerinfo[i].pos.r = std::round(playerinfo[i].pos.r);
					vel_x[i] = 0;
					vel_y[i] = +1;
				}
				break;
			case PlayerState::LEFT:
				playerinfo[i].state = PlayerState::LEFT;
				if ((1.2 < playerinfo[i].pos.c && playerinfo[i].pos.c < 2.8) ||
					(3.2 < playerinfo[i].pos.c && playerinfo[i].pos.c < 4.8) ||
					(5.2 < playerinfo[i].pos.c && playerinfo[i].pos.c < 6.8)) {
					vel_x[i] = 0;
					vel_y[i] = 0;
				}//  ��ź �浹 ó�� -0.1 0.2;

				else if (ClosedTiles[(int)(playerinfo[i].pos.r -0.1 )][(int)(playerinfo[i].pos.c +0.4)] == BOMB) {
					vel_x[i] = 0;
					vel_y[i] = 0;

				}

				else {
					playerinfo[i].pos.c = std::round(playerinfo[i].pos.c);
					vel_x[i] = -1;
					vel_y[i] = 0;
				}
				break;
			case PlayerState::RIGHT:
				playerinfo[i].state = PlayerState::RIGHT;
				if ((1.2 < playerinfo[i].pos.c && playerinfo[i].pos.c < 2.8) ||
					(3.2 < playerinfo[i].pos.c && playerinfo[i].pos.c < 4.8) ||
					(5.2 < playerinfo[i].pos.c && playerinfo[i].pos.c < 6.8)) {
					vel_x[i] = 0;
					vel_y[i] = 0;
				}//  ��ź �浹 ó��  +0.1 0.99;
				else if (ClosedTiles[(int)(playerinfo[i].pos.r + 1)][(int)(playerinfo[i].pos.c )] == BOMB) {
					vel_x[i] = 0;
					vel_y[i] = 0;

				}
				else {
					playerinfo[i].pos.c = std::round(playerinfo[i].pos.c);
					vel_x[i] = 1;
					vel_y[i] = 0;

				}
				break;


			}
			float t = 1.f / 10.f;
			playerinfo[i].pos.r = playerinfo[i].pos.r + vel_x[i] * t;
			playerinfo[i].pos.c = playerinfo[i].pos.c + vel_y[i] * t;

			if (playerinfo[i].pos.r <= 1) {
				playerinfo[i].pos.r = 1;
			}
			if (playerinfo[i].pos.c <= 1) {
				playerinfo[i].pos.c = 1;
			}
			if (7 <= playerinfo[i].pos.c) {
				playerinfo[i].pos.c = 7;
			}
			if (7 <= playerinfo[i].pos.r) {
				playerinfo[i].pos.r = 7;
			}

		}
		//	// ��ź ���� �� ����	
		int zero_bombs{};
		int zero_explosions{};

		for (auto& bomb : bombs) {

			if (bomb.bombinfo.bomb_count_down == 0) {
				//������ ���� �� ����
				playerinfo[bomb.id].bomb_count++;
				zero_bombs++;

				// �ڱ� ��ġ
				SendExplosiveInfo tmp;
				tmp.pos.r = bomb.bombinfo.pos.r;
				tmp.pos.c = bomb.bombinfo.pos.c;
				explosions.emplace_back(tmp, 2000);
				// y�� ���� x�� ������ ���� Ȯ��

				// �ڽ� ���� ������
				for (int i = bomb.bombinfo.pos.r + 1; i < 9; ++i) {
					if (ClosedTiles[i][(int)bomb.bombinfo.pos.c] != true) {
						SendExplosiveInfo tmp;
						tmp.pos.r = i;
						tmp.pos.c = bomb.bombinfo.pos.c;
						explosions.emplace_back(tmp, 2000);
					}
					else break;
				}
				// �ڽ� ���� ����
				for (int i = bomb.bombinfo.pos.r - 1; i > 0; --i) {
					if (ClosedTiles[i][(int)bomb.bombinfo.pos.c] != true) {
						SendExplosiveInfo tmp;
						tmp.pos.r = i;
						tmp.pos.c = bomb.bombinfo.pos.c;
						explosions.emplace_back(tmp, 2000);
					}
					else break;
				}
				//x�� ���� y�� ������ ���� Ȯ��
				// �ڽź��� �Ʒ���
				for (int i = bomb.bombinfo.pos.c + 1; i < 9; ++i) {
					if (ClosedTiles[(int)bomb.bombinfo.pos.r][i] != true) {
						SendExplosiveInfo tmp;
						tmp.pos.r = bomb.bombinfo.pos.r;
						tmp.pos.c = i;
						explosions.emplace_back(tmp, 2000);
					}
					else break;
				}
				// �ڽź��� ����
				for (int i = bomb.bombinfo.pos.c - 1; i >= 0; --i) {
					if (ClosedTiles[(int)bomb.bombinfo.pos.r][i] != true) {
						SendExplosiveInfo tmp;
						tmp.pos.r = bomb.bombinfo.pos.r;
						tmp.pos.c = i;
						explosions.emplace_back(tmp, 2000);
					}
					else break;
				}


			}

		}


		// ���� ������ ĳ���� �浹 üũ

		for (int i = 0; i < number_of_clients; ++i) {
			if (playerinfo[i].state == PlayerState::DEAD)continue;
			for (const auto& explosion : explosions) {

				if ((std::abs(explosion.explosiveinfo.pos.r - playerinfo[i].pos.r) < 0.9)
					&& (std::abs(explosion.explosiveinfo.pos.c - playerinfo[i].pos.c) < 0.9)) {

					if (playerinfo[i].no_damage_count == 0) {
						playerinfo[i].life_count--;
						playerinfo[i].no_damage_count = 3000;
					}
				}
				if ((std::abs(explosion.explosiveinfo.pos.c - playerinfo[i].pos.c) < 0.9) &&
					(std::abs(explosion.explosiveinfo.pos.r - playerinfo[i].pos.r) < 0.9)) {
					if (playerinfo[i].no_damage_count == 0) {
						playerinfo[i].life_count--;
						playerinfo[i].no_damage_count = 3000;
					}
				}


			}

		}
		// ���� ����
		for (auto& bomb : bombs) {
			for (const auto& explosion : explosions) {
				if (bomb.bombinfo.pos.r == explosion.explosiveinfo.pos.r &&
					bomb.bombinfo.pos.c == explosion.explosiveinfo.pos.c) {
					bomb.bombinfo.bomb_count_down = 0;
				}
			}
		}

		alivePlayer = 0;
		for (int i = 0; i < number_of_clients; ++i) {
			if (playerinfo[i].state != PlayerState::DEAD) {
				alivePlayer++;
			}
		}

		// ���� �ð� ���� �� ����
		for (int i = 0; i < number_of_clients; ++i) {
			if (playerinfo[i].state == PlayerState::DEAD)continue;
			// ���� ó��

			if (3000 < playerinfo[i].no_damage_count)
				playerinfo[i].no_damage_count = 0;

			if (playerinfo[i].no_damage_count != 0)
				playerinfo[i].no_damage_count -= 32;

		}

		//��ź �ð��� ����
		for (auto& bomb : bombs) {
			
			//���� ó��
			if (3000 < bomb.bombinfo.bomb_count_down) {
				bomb.bombinfo.bomb_count_down = 0;
			}

			if (bomb.bombinfo.bomb_count_down != 0) {
				bomb.bombinfo.bomb_count_down -= 32;
			}

		}

		// ���� �ð��� ����
		for (auto& explosion : explosions) {
			//���� ó��
			if (2000 < explosion.explosive_countdown) {
				explosion.explosive_countdown = 0;
			}

			if (explosion.explosive_countdown != 0) {
				explosion.explosive_countdown -= 32;
			}

		}


		// ��ź ���� ����
		for (int i = 0; i < zero_bombs; ++i) {
			bombLock.lock();
			ClosedTiles[(int)bombs.front().bombinfo.pos.r][(int)bombs.front().bombinfo.pos.c] = false;
			bombLock.unlock();
			bombs.pop_front();
		}
		zero_bombs = 0;

		g_lock.lock();
		for (auto explosion : explosions) {
			if (explosion.explosive_countdown == 0)
				zero_explosions++;

		}
		g_lock.unlock();
		// ���� ��ġ ����
		for (int i = 0; i < zero_explosions; ++i) {
			explosions.pop_front();
		}
		zero_explosions = 0;

		game_packet::SC_WorldState WorldPacket{};
		// ��Ŷ �������� 
		WorldPacket.player_count = alivePlayer;
		WorldPacket.bomb_count = bombs.size();
		WorldPacket.explosive_count = explosions.size();

		char* cur_ptr = WorldPacket.buf;

		//���� ���� ����
		for (auto& explosion : explosions) {
			memcpy(cur_ptr, &explosion, sizeof(SendExplosiveInfo));
			cur_ptr += sizeof(SendExplosiveInfo);
		}

		//��ź ���� ����
		for (auto& bomb : bombs) {
			memcpy(cur_ptr, &bomb.bombinfo, sizeof(SendBombInfo));
			cur_ptr += sizeof(SendBombInfo);
		}


		//�÷��̾� ���� ����
		for (int i = 0; i < number_of_clients; ++i) {
			if (playerinfo[i].state == PlayerState::DEAD) continue;
			playerinfo[i].id = i;
			memcpy(cur_ptr, &playerinfo[i], sizeof(PlayerInfo));
			cur_ptr += sizeof(PlayerInfo);

		}


		for (int i = 0; i < number_of_clients; ++i) {
			int retval = send(clients[i].client, (char*)&WorldPacket, sizeof(WorldPacket), 0);
			if (retval == SOCKET_ERROR) {
				error_display("send");
			}
		}

		for (int i = 0; i < number_of_clients; ++i) {
			if (playerinfo[i].life_count == 0) {
				playerinfo[i].state = PlayerState::DEAD;
			}
		}

		if (alivePlayer <= 1) {
			return 0 ;
		}


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
	SceneCheck = SceneID::LOBBY;

	while (1) {
		//accept()
		// ���� ���۰� 4���̻� ���Ʈ �Ұ�.
		if (number_of_clients >= 4 || SceneCheck != SceneID::LOBBY) continue;
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		// ���⼭ ���� ���� ���ӽ��� �Ǿ����� ���� ����
		if (SceneCheck != SceneID::LOBBY ) {
			// ���� ����
			closesocket(client_sock);
			continue;
		}
		if (client_sock == INVALID_SOCKET) {
			error_display((char*)"accept()");
			break;
		}
		cout << "accept\n";
	
		// �ʱ�ȭ
		clients[number_of_clients].client = client_sock;
		clients[number_of_clients].index = number_of_clients;
		
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