#pragma once
#include <chrono>
#include "GameObject.h"

class ObjectContainer
{
public:
	PlayerInfo playerInfo[4];
	std::mutex lock;

public:
	std::vector<StaticObject>	m_StaticObjects;
	std::list<DynamicObject>	m_DynamicObjects;

	DynamicObject* characters[4];

	void AddStaticObject(BitmapKey key, Vector2 pos);
	DynamicObject* AddDynamicObject(BitmapKey key, Vector2 pos);
	DynamicObject* AddCharater(int id, Vector2 pos);

	void Update();
	
	void Reset();
};
