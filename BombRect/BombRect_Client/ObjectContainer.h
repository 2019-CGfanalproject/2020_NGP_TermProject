#pragma once
#include "GameObject.h"

class ObjectContainer
{
public:
	std::vector<StaticObject> m_StaticObjects;
	std::list<DynamicObject> m_DynamicObjects;

	void AddStaticObject(BitmapKey key, Vector2 pos);
	// void AddDynamicObject();
};
