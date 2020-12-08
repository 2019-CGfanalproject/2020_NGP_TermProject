#pragma once

constexpr LONG CLIENT_WIDTH = 800;
constexpr LONG CLIENT_HEIGHT = 800;

class ClientRect : public RECT {
public:
	LONG GetWidth() {
		return right - left;
	}
	LONG GetHeight() {
		return bottom - top;
	}
};

enum class BitmapKey {
	BACKGOURND,
	CLOSED_TILE,
	CHARACTER_CYAN,
	CHARACTER_GREEN,
	CHARACTER_RED,
	CHARACTER_YELLOW,
	TITLE,
	IPADDR_EDITBAR,
	READY_BUTTON,
	
	BOMB,
	BOMB_WHITE,
	EXPLOSION,

	UI_HEART,
	UI_RED,
	UI_YELLOW,
	UI_GREEN,
	UI_CYAN,

	TEAM_ICON,
	PLAYER_SLOT,

	LOGIN,
	READY_TEXT,
	RESULT,

	IPADDR_BAR,
	IPADDR_SELECTED_BAR,
	NICKNAME_BAR,
	NICKNAME_SELECTED_BAR,

	CHATTING_REGION,
	CHATTING_INPUT_REGION,
};

enum class FontKey {
	LOGIN,
	CHATTING,
	NICKNAME,
};

template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease) {
	if (*ppInterfaceToRelease != NULL) {
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

inline BitmapKey GetBitmapKeyFrom(int id) {
	switch (id) {
	case 0:
		return BitmapKey::CHARACTER_RED;
	case 1:
		return BitmapKey::CHARACTER_YELLOW;
	case 2:
		return BitmapKey::CHARACTER_GREEN;
	case 3:
		return BitmapKey::CHARACTER_CYAN;
	default:
		return BitmapKey::CHARACTER_RED;
	}
}