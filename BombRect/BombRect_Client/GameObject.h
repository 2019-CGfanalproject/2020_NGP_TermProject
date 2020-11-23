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
};

class StaticObject : public GameObject {
public:
	StaticObject(BitmapKey key, Vector2 pos) 
		: GameObject(key, pos) { }


};

class DynamicObject : public GameObject {

};