#pragma once
#include <chrono>
#include "GameObject.h"
#include "../packets.h"


// ���� ������ ���¸� �����ϰ� �ؼ��Ѵ�.
// �������� ���� ������ ���� ������Ʈ�� �����Ѵ�.
class ObjectContainer	// GameDatamanager
{
public:
	game_packet::SC_WorldState m_WorldState;

	float degree[4];
	std::mutex lock;

public:
	ObjectContainer() {
		degree[0] = 0.f;
		degree[1] = 180.f;
		degree[2] = 0.f;
		degree[3] = 180.f;
	};
	~ObjectContainer() {};

	std::vector<StaticObject>	m_StaticObjects;
	std::list<DynamicObject>	m_DynamicObjects;

	DynamicObject* characters[4];

	void AddStaticObject(BitmapKey key, Vector2 pos);
	DynamicObject* AddDynamicObject(BitmapKey key, Vector2 pos);
	DynamicObject* AddDynamicObject(
		BitmapKey key, Vector2 pos, 
		float degree, float opacity);

	inline void SetWorldState(game_packet::SC_WorldState& world_state) {
		lock.lock();
		m_WorldState = world_state;
		lock.unlock();
	}

	void Update();
	
	void Reset();
};
