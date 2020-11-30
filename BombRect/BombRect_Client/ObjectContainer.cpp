#include "pch.h"
#include "ObjectContainer.h"
#include "GameObject.h"

BitmapKey GetBitmapKeyFrom(int id) {
	switch (id) {
	case 0:
		return BitmapKey::CHARACTER_RED;
	case 1:
		return BitmapKey::CHARACTER_YELLOW;
	case 2:
		return BitmapKey::CHARACTER_CYAN;
	case 3:
		return BitmapKey::CHARACTER_GREEN;
	default:
		return BitmapKey::CHARACTER_RED;
	}
}

void ObjectContainer::AddStaticObject(BitmapKey key, Vector2 pos)
{
	m_StaticObjects.emplace_back(key, pos);
}

DynamicObject* ObjectContainer::AddDynamicObject(BitmapKey key, Vector2 pos)
{
	m_DynamicObjects.emplace_back(key, pos);
	return &m_DynamicObjects.back();
}

Vector2 ui_pos[4] = {
	{70,	10	},
	{680,	10	},
	{70,	730	},
	{680,	730	},
};

void ObjectContainer::Update()
{
	m_DynamicObjects.clear();

	// 현재 패킷을 해석해 업데이트한다.
	lock.lock();
	char* ptr = m_WorldState.buf;

	PlayerInfo player_info;
	SendBombInfo bomb_info;
	SendExplosiveInfo explosion_info;

	// for debug : print count
	
	OutputDebugString(L"player count: ");
	OutputDebugString(std::to_wstring(m_WorldState.player_count).c_str());
	OutputDebugString(L"\n");

	OutputDebugString(L"bomb count: ");
	OutputDebugString(std::to_wstring(m_WorldState.bomb_count).c_str());
	OutputDebugString(L"\n");

	OutputDebugString(L"explosive count: ");
	OutputDebugString(std::to_wstring(m_WorldState.explosive_count).c_str());
	OutputDebugString(L"\n");
	

	int map_padding = 40;

	for (int i = 0; i < m_WorldState.player_count; ++i) {
		memcpy(&player_info, ptr, sizeof(PlayerInfo));
		ptr += sizeof(PlayerInfo);

		AddDynamicObject(GetBitmapKeyFrom(player_info.id), Vector2(player_info.pos) + map_padding);

		for (int life = 0; life < player_info.life_count; ++life) {

			Vector2 pos{ ui_pos[i].x + (60 * life * ((i % 2) ? -1 : 1)) , ui_pos[i].y };
			AddDynamicObject(BitmapKey::UI_HEART, pos);
		}

		//OutputDebugString(std::to_wstring(player_info.pos.r).c_str());
		//OutputDebugString(L"\n");
	}

	for (int i = 0; i < m_WorldState.bomb_count; ++i) {
		memcpy(&bomb_info, ptr, sizeof(SendBombInfo));
		ptr += sizeof(SendBombInfo);

		AddDynamicObject(BitmapKey::BOMB, Vector2(bomb_info.pos) + map_padding);
	}

	for (int i = 0; i < m_WorldState.explosive_count; ++i) {
		memcpy(&explosion_info, ptr, sizeof(TilePos));
		ptr += sizeof(TilePos);

		AddDynamicObject(BitmapKey::EXPLOSION, Vector2(explosion_info.pos) + map_padding);
	}

	lock.unlock();
}

void ObjectContainer::Reset()
{
	m_StaticObjects.clear();
	m_DynamicObjects.clear();
}
