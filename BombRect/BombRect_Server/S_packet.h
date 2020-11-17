#pragma once
namespace login_packet {
#pragma pack(1)
	struct CS_Nickname {
		TCHAR buf[16];
	};
#pragma pack()
}

namespace lobby_packet {
	enum class PacketType : char {
		READY,
		CHATING,
		GAME_START
	};

#pragma pack(1)
	struct Ready {
		PacketType type;
		unsigned short size;
	};

	struct Chatting {
		PacketType type;
		unsigned short size;
		char string[256];
	};

	struct SC_GameStart {
		PacketType type;
		unsigned short size;
	};
#pragma pack()
}

enum PlayerState;
namespace game_packet {
	enum class PacketType {
		PlayerState,
		Bomb,
	};
#pragma pack(1)
	struct CS_PlayerState {
		PacketType type;
		PlayerState state;
	};

	struct CS_Bomb {
		PacketType type;
	};

	struct SC_WorldState {
		unsigned short player;
		unsigned short bomb;
		unsigned short explosive;
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