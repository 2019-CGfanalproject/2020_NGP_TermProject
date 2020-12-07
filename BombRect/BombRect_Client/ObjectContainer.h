#pragma once
#include <chrono>
#include "GameObject.h"
#include "../packets.h"

// ���� ������ ���¸� �����ϰ� �ؼ��Ѵ�.
// �������� ���� ������ ���� ������Ʈ�� �����Ѵ�.
class ObjectContainer	// GameDatamanager
{
public:
	std::mutex m_Lock;
	lobby_packet::Nickname m_Nicknames[4];
	bool m_ReadyState[4];
	std::vector<TextObject*> m_Chattings;

	game_packet::SC_WorldState m_WorldState;

	float degree[4];

	std::list<int> ranking;
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
		// ������ ������������
		/*TextObject* new_chat = AddText();
		new_chat->m_Text = str;
		new_chat->m_ = str;
		new_chat->m_ = str;
		new_chat->m_ = str;
		new_chat->m_ = str;*/
		m_Lock.unlock();
	}

	TextObject* AddText();
};
