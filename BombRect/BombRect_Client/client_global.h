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
};

template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease) {
	if (*ppInterfaceToRelease != NULL) {
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}
