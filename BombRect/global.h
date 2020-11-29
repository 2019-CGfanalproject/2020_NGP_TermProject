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
	unsigned int life_count{ 3 };
	unsigned int bomb_count{ 3 };
	unsigned int no_damage_count;	
};

struct SendBombInfo {
	TilePos pos;
	unsigned int bomb_count_down;
};

struct SendExplosiveInfo {
	TilePos pos;
	SendExplosiveInfo(TilePos& pos)  {
		this->pos = pos;
	}

};