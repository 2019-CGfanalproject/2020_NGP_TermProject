#pragma once

struct Vector2 {
	int x;
	int y;
	Vector2(int x, int y) : x(x), y(y) { }
	Vector2 operator+(const Vector2 rhs) {
		return Vector2(x + rhs.x, y + rhs.y);
	}
};

struct TilePos {
	float r;
	float c;
	TilePos() {};
	TilePos(float r, float c) : r(r), c(c) { }
	TilePos operator+(const TilePos rhs) {
		return TilePos(r + rhs.r, c + rhs.c);
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
	unsigned int life_count;
	unsigned int bomb_count;
	unsigned int no_damage_count;
	
};
