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

DynamicObject* ObjectContainer::AddCharater(int id, Vector2 pos)
{
	BitmapKey key;

	switch (id) {
	case 0:
		key = BitmapKey::CHARACTER_RED;
		break;
	case 1:
		key = BitmapKey::CHARACTER_YELLOW;
		break;
	case 2:
		key = BitmapKey::CHARACTER_GREEN;
		break;
	case 3:
		key = BitmapKey::CHARACTER_CYAN;
		break;
	defalut:
		OutputDebugStringA("잘못된 플레이어 ID\n");
		exit(-1);
	}

	Character ch{ id, key, pos };
	
	m_DynamicObjects.push_back(ch);

	return &m_DynamicObjects.back();
}

void ObjectContainer::Update()
{
	m_DynamicObjects.clear();

	// 현재 패킷을 해석해 업데이트한다.
	lock.lock();
	char* ptr = m_WorldState.buf;

	PlayerInfo player_info;
	SendBombInfo bomb_info;
	SendExplosiveInfo explosion_info;

	for (int i = 0; i < m_WorldState.player_count; ++i) {
		memcpy(&player_info, ptr, sizeof(PlayerInfo));
		ptr += sizeof(PlayerInfo);

		AddDynamicObject(GetBitmapKeyFrom(player_info.id), Vector2(player_info.pos));
		OutputDebugString(std::to_wstring(player_info.pos.r).c_str());
		OutputDebugString(L"\n");
	}

	for (int i = 0; i < m_WorldState.bomb_count; ++i) {
		memcpy(&bomb_info, ptr, sizeof(player_info));
		ptr += sizeof(SendBombInfo);

		AddDynamicObject(BitmapKey::BOMB, Vector2(bomb_info.pos));
	}

	for (int i = 0; i < m_WorldState.explosive_count; ++i) {
		memcpy(&explosion_info, ptr, sizeof(TilePos));
		ptr += sizeof(TilePos);

		AddDynamicObject(BitmapKey::EXPLOSION, Vector2(explosion_info.pos));
	}

	lock.unlock();

	//lock.lock();
	//for (int i = 0; i < 100; ++i) {
	//	if (explo_info[i].r == -1) break;
	//	AddDynamicObject(BitmapKey::EXPLOSION,
	//		Vector2{ (int)explo_info[i].r * 80 + 40, (int)explo_info[i].c * 80  + 40});
	//}
	//lock.unlock();
}

void ObjectContainer::Reset()
{
	m_StaticObjects.clear();
	m_DynamicObjects.clear();
}
