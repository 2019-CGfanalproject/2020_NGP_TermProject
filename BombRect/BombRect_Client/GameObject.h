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

