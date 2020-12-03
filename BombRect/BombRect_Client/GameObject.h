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

class TextEditor : public GameObject {
	// 프레임은 포함하지 않는다. 따로 그려줘야함

	std::wstring m_Text;
};