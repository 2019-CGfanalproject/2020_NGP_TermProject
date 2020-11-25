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
};

enum class Input : int {
	ARROW_UP,
	ARROW_DOWN,
	ARROW_LEFT,
	ARROW_RIGHT,
	SPACEBAR,
	_COUNT,
};

template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease) {
	if (*ppInterfaceToRelease != NULL) {
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}