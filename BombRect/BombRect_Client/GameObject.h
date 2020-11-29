#pragma once

class GameObject
{
protected:
	BitmapKey m_Key;
	Vector2 m_Pos;

	GameObject(BitmapKey key, Vector2 pos) 
		: m_Key(key), m_Pos(pos) { }

public:
	Vector2 GetPos() const {
		return m_Pos;
	}

	BitmapKey GetBitmapKey() const {
		return m_Key;
	}

	void SetPos(TilePos pos) {
		m_Pos.x = pos.r * 80 + 40;
		m_Pos.y = pos.c * 80 + 40;
	}
};

class StaticObject : public GameObject {
public:
	StaticObject(BitmapKey key, Vector2 pos)   
		: GameObject(key, pos) { }
};

class DynamicObject : public GameObject {
public:
	DynamicObject(BitmapKey key, Vector2 pos)
		: GameObject(key, pos) { }

	virtual void Update(Vector2 pos) { };
};

class Character : public DynamicObject {
	int m_UserID;

public:
	Character(int id, BitmapKey key, Vector2 pos);        
	void Update(Vector2 pos) override;

};

class Player {
	DynamicObject* m_Object;

public:
	PlayerState state = PlayerState::IDLE;
	bool m_InputControl[5];

	Player(DynamicObject* object)
		: m_Object(object),                          
		state(PlayerState::IDLE)
	{ 
		for (int i = 0; i < (int)Input::_COUNT; ++i) {
			m_InputControl[i] = false;
		}
	}

	PlayerState CalcState(WPARAM cur_key) {
		bool is_idle = true;
		for (int i = 0; i < 4; ++i) {
			if (m_InputControl[i]) is_idle = false;
		}
		if (is_idle) return PlayerState::IDLE;

		switch (cur_key) {
		case VK_LEFT:
			return PlayerState::LEFT;
		case VK_UP:
			return PlayerState::UP;
		case VK_RIGHT:
			return PlayerState::RIGHT;
		case VK_DOWN:
			return PlayerState::DOWN;
		}
	}

	void SetPos(TilePos& pos) {
		m_Object->SetPos(pos);
	}
};
