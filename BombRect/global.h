#pragma once

struct TilePos {
	float r;
	float c;
	TilePos() {};
	TilePos(float r, float c) : r(r), c(c) { }
	TilePos operator+(const TilePos rhs) {
		return TilePos(r + rhs.r, c + rhs.c);
	}
};

struct Vector2 {
	int x;
	int y;
	Vector2(int x, int y) : x(x), y(y) { }
	Vector2(TilePos pos)
		: x(pos.r * 80), y(pos.c * 80) { }

	Vector2 operator+(const Vector2 rhs) {
		return Vector2(x + rhs.x, y + rhs.y);
	}

	Vector2 operator+(const int value) {
		return Vector2(x + value, y + value);
	}

};

enum class SceneID : int {
	LOGIN,
	LOBBY,
	GAME,
	RESULT,
	COUNT,
};

enum class PlayerState {
	IDLE,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	DEAD
};

struct PlayerInfo {
	int id;
	PlayerState state;
	TilePos pos;
	unsigned int life_count{ 3 };
	unsigned int bomb_count{ 3 };
	unsigned int no_damage_count;	
};

struct SendBombInfo {
	TilePos pos;
	unsigned int bomb_count_down{ 3 };
};

struct SendExplosiveInfo {
	TilePos pos;
	SendExplosiveInfo() : pos() { }
	SendExplosiveInfo(TilePos& pos) : pos(pos) { }
};
