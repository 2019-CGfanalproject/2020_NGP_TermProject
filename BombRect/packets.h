#pragma once

namespace lobby_packet {
	enum class PacketType : char {
		READY,
		CHATING,
		GAME_START,
		LOBBY_INFO,
	};

#pragma pack(1)
	struct Nickname {
		unsigned int id;
		TCHAR name[16];
	};

	struct LobbyInfo {
		PacketType type;
		unsigned short size;
		Nickname users[4];
	};

	struct Ready {
		PacketType type;
		unsigned short size;
	};

	struct Chatting {
		PacketType type;
		unsigned short size;
		TCHAR string[256];
	};

	struct SC_GameStart {
		PacketType type;
		unsigned short size;
	};
#pragma pack()
}

namespace game_packet {
	enum class PacketType {
		PlayerState,
		Bomb,
		GameOver,
	};
#pragma pack(1)
	struct CS_PlayerState {
		PacketType type;
		PlayerState state;
	};

	struct CS_Bomb {
		PacketType type;
	};

	struct CS_GameOver {
		PacketType type;
	};

	struct SC_WorldState {
		unsigned short player_count; 
		unsigned short bomb_count;  
		unsigned short explosive_count; 
		char buf[1024];
	};
#pragma pack()
}

namespace result_packet {
#pragma pack(1)
	struct TimeOver {
		bool time_over;
	};
#pragma pack()
}
