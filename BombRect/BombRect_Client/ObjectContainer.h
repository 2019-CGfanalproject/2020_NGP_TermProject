#pragma once
#include <chrono>
#include "GameObject.h"
#include "../packets.h"


// 게임 월드의 상태를 저장하고 해석한다.
// 렌더링을 위한 정보를 담은 오브젝트를 관리한다.
class ObjectContainer	// GameDatamanager
{
public:
	game_packet::SC_WorldState m_WorldState;

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

	inline void SetWorldState(game_packet::SC_WorldState& world_state) {
		lock.lock();
		m_WorldState = world_state;
		lock.unlock();
	}

	void Update();
	
	void Reset();
};
