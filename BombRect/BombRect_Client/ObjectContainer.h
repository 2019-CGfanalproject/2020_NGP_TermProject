#pragma once
#include <chrono>
#include "GameObject.h"
#include "../packets.h"

// 게임 월드의 상태를 저장하고 해석한다.
// 렌더링을 위한 정보를 담은 오브젝트를 관리한다.
class ObjectContainer	// GameDatamanager
{
public:
	std::mutex m_Lock;
	lobby_packet::Nickname m_Nicknames[4];
	game_packet::SC_WorldState m_WorldState;

	float degree[4];

	int ranking[4]{};
	int curr_rank = 0;

public:
	ObjectContainer();
	~ObjectContainer() { };

public:
	std::vector<StaticObject>	m_StaticObjects;
	std::list<DynamicObject>	m_DynamicObjects;
	std::vector<TextObject*>		m_Texts;
	TextObject m_TextObjects[4];

public:
	void AddStaticObject(BitmapKey key, Vector2 pos);
	DynamicObject* AddDynamicObject(BitmapKey key, Vector2 pos);
	DynamicObject* AddDynamicObject(BitmapKey key, Vector2 pos, float degree, float opacity);

	void NicknameUpdate();

	void Update();
	
	void Reset();

	inline void SetWorldState(game_packet::SC_WorldState& world_state) {
		m_Lock.lock();
		m_WorldState = world_state;
		m_Lock.unlock();
	}

	inline void SetNicknames(lobby_packet::Nickname* nicknames) {
		m_Lock.lock();
		memcpy(m_Nicknames, nicknames, sizeof(lobby_packet::Nickname) * 4);
		m_Lock.unlock();
	}

	TextObject* AddText();
};
