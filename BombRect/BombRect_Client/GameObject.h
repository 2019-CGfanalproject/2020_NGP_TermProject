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
	float m_Degree;
	float m_Opacity;

	DynamicObject(BitmapKey key, Vector2 pos)
		: GameObject(key, pos),
		m_Degree(0),
		m_Opacity(1.0)
	{ }

	virtual void Update(Vector2 pos) { };

	void SetDegree(float degree) {
		m_Degree = degree;
	}

	void SetOpacity(float opacity) {
		m_Opacity = opacity;
	}
};

class TextObject {
public:
	String m_Text;
	int m_Left;
	int m_Top;
	int m_Right;
	int m_Bottom;

	FontKey font;
};
