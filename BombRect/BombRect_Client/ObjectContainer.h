#pragma once
#include <chrono>
#include "GameObject.h"

class ObjectContainer
{
public:
	PlayerInfo playerInfo[4];
	TilePos explo_info[100];
	std::mutex lock;

public:
	ObjectContainer() {
		for (int i = 0; i < 100; ++i) {
			explo_info[i].r = -1;
			explo_info[i].c = -1;
		}
	};
	~ObjectContainer() {};

	std::vector<StaticObject>	m_StaticObjects;
	std::list<DynamicObject>	m_DynamicObjects;

	DynamicObject* characters[4];

	void AddStaticObject(BitmapKey key, Vector2 pos);
	DynamicObject* AddDynamicObject(BitmapKey key, Vector2 pos);
	DynamicObject* AddCharater(int id, Vector2 pos);

	void Update();
	
	void Reset();
};
