#pragma once
enum class SceneID : int {
	LOGIN,
	LOBBY,
	GAME,
	RESULT,
	COUNT,
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
