#pragma once
#include <chrono>
#include "GameObject.h"
#include "../packets.h"

// 게임 월드의 상태를 저장하고 해석한다.
// 렌더링을 위한 정보를 담은 오브젝트를 관리한다.
class ObjectContainer	// GameDatamanager
{
	Vector2 slot_nickname_pos[4] = {
		{325,	25 + 15 },
		{575,	25 + 15 },
		{325,	325 + 15 },
		{575,	325 + 15 }
	};

public:
	std::mutex m_Lock;
	bool m_ReadyState[4];

	std::vector<String*> m_Chattings;

	lobby_packet::Nickname m_Nicknames[4];
	game_packet::SC_WorldState m_WorldState;

	float degree[4];

	std::list<int> ranking;

public:
	ObjectContainer();
	~ObjectContainer() { };

public:
	// rederer에서 참조하는 객체들
	std::vector<StaticObject>	m_StaticObjects;
	std::list<DynamicObject>	m_DynamicObjects;
	std::vector<TextObject*>	m_Texts;

public:
	void AddStaticObject(BitmapKey key, Vector2 pos);
	DynamicObject* AddDynamicObject(BitmapKey key, Vector2 pos);
	DynamicObject* AddDynamicObject(BitmapKey key, Vector2 pos, float degree, float opacity);

	void UpdateLobby();

	void Update();
	
	void Reset();

	inline void SetWorldState(game_packet::SC_WorldState& world_state) {
		m_Lock.lock();
		m_WorldState = world_state;
		m_Lock.unlock();
	}

	inline void SetNicknames(lobby_packet::Nickname* nicknames) {
		m_Lock.lock();

		for (int i = 0; i < 4; ++i) {
			if (m_Nicknames[i].id != nicknames[i].id) {	// 변화한 것만 새로 닉네임을 추가해줌
				TextObject* name = AddText();
				name->m_Text = nicknames[i].name;
				name->m_Left = slot_nickname_pos[i].x;
				name->m_Top = slot_nickname_pos[i].y;
				name->m_Right = slot_nickname_pos[i].x + 200;
				name->m_Bottom = slot_nickname_pos[i].y + 50;
				name->font = FontKey::NICKNAME;
				name->is_align_center = true;
			}

			// 레디정보반영
			m_ReadyState[i] = nicknames[i].Ready;
		}

		memcpy(m_Nicknames, nicknames, sizeof(lobby_packet::Nickname) * 4);

		m_Lock.unlock();
	}

	inline void SetReady(int id) {
		m_Lock.lock();
		m_ReadyState[id] = !m_ReadyState[id];
		m_Lock.unlock();
	}

	void AddChatting(const TCHAR* str) {
		m_Lock.lock();
		// m_Chattings.emplace_back(str);
		m_Lock.unlock();
	}

	TextObject* AddText();
};
