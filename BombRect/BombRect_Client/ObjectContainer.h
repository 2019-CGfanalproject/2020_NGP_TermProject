#pragma once
#include <chrono>
#include "GameObject.h"

class ObjectContainer
{
public:
	std::vector<StaticObject>	m_StaticObjects;
	std::list<DynamicObject>	m_DynamicObjects;
	DynamicObject* m_Players[4];

	void AddStaticObject(BitmapKey key, Vector2 pos);
	DynamicObject* AddDynamicObject(BitmapKey key, Vector2 pos);
};
