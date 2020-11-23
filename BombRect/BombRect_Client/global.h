#pragma once
constexpr LONG CLIENT_WIDTH = 800;
constexpr LONG CLIENT_HEIGHT = 800;

enum class SceneID : int {
	LOGIN,
	LOBBY,
	GAME,
	RESULT,
	COUNT,
};

enum class BitmapKey {
	BACKGOURND,
	CLOSED_TILE,
};

class ClientRect : public RECT {
public:
	LONG GetWidth() {
		return right - left;
	}
	LONG GetHeight() {
		return bottom - top;
	}
};

struct Vector2 {
	int x;
	int y;
	Vector2(int x, int y) : x(x), y(y) { }
	Vector2 operator+(const Vector2 rhs) {
		return Vector2(x + rhs.x, y + rhs.y);
	}
};

struct TilePos {
	int r;
	int c;
};

template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease) {
	if (*ppInterfaceToRelease != NULL) {
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}
