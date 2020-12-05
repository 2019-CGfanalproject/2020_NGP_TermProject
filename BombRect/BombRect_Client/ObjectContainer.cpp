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
		return BitmapKey::CHARACTER_GREEN;
	case 3:
		return BitmapKey::CHARACTER_CYAN;
	default:
		return BitmapKey::CHARACTER_RED;
	}
}

Vector2 ui_pos[4] = {
	{70,	10	},
	{680,	10	},
	{70,	730	},
	{680,	730	},
};
Vector2 slot_player_pos[4] = {
	{325 + 60,	25 + 60},
	{575 + 60,	25 + 60},
	{325 + 60,	325 + 60},
	{575 + 60,	325 + 60},
};

Vector2 slot_nickname_pos[4] = {
	{325,	25},
	{575,	25},
	{325,	325},
	{575,	325},
};

ObjectContainer::ObjectContainer()
{
	degree[0] = 0.f;
	degree[1] = 180.f;
	degree[2] = 0.f;
	degree[3] = 180.f;

	for (int i = 0; i < 4; ++i) {
		m_TextObjects[i].m_Left = slot_nickname_pos[i].x;
		m_TextObjects[i].m_Top = slot_nickname_pos[i].y;
		m_TextObjects[i].m_Right = slot_nickname_pos[i].x + 200;
		m_TextObjects[i].m_Bottom = slot_nickname_pos[i].y + 50;
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

DynamicObject* ObjectContainer::AddDynamicObject(
	BitmapKey key, Vector2 pos, 
	float degree, float opacity
)
{
	DynamicObject object{ key, pos };
	object.SetDegree(degree);
	object.SetOpacity(opacity);

	m_DynamicObjects.push_back(object);

	return &m_DynamicObjects.back();
}



void ObjectContainer::NicknameUpdate()
{
	m_DynamicObjects.clear();

	m_Lock.lock();
	for (int i = 0; i < 4; ++i) {
		if (m_Nicknames[i].id == -1) continue;		// �� ������ id�� -1�� ä���� ������ ��
		AddDynamicObject(
			GetBitmapKeyFrom(m_Nicknames[i].id),
			slot_player_pos[i]
		);
		m_TextObjects[i].m_Left = slot_nickname_pos[i].x;
		m_TextObjects[i].m_Top = slot_nickname_pos[i].y;
		m_TextObjects[i].m_Right = slot_nickname_pos[i].x + 200;
		m_TextObjects[i].m_Bottom = slot_nickname_pos[i].y + 50;

		m_TextObjects[i].m_Text = m_Nicknames[i].name;
	}
	m_Lock.unlock();
}

void ObjectContainer::Update()
{
	m_DynamicObjects.clear();

	// ���� ��Ŷ�� �ؼ��� ������Ʈ�Ѵ�.
	m_Lock.lock();
	char* ptr = m_WorldState.buf;

	PlayerInfo player_info;
	SendBombInfo bomb_info;
	SendExplosiveInfo explosion_info;

	// for debug : print count
	/*
	OutputDebugString(L"player count: ");
	OutputDebugString(std::to_wstring(m_WorldState.player_count).c_str());
	OutputDebugString(L"\n");

	OutputDebugString(L"bomb count: ");
	OutputDebugString(std::to_wstring(m_WorldState.bomb_count).c_str());
	OutputDebugString(L"\n");

	OutputDebugString(L"explosive count: ");
	OutputDebugString(std::to_wstring(m_WorldState.explosive_count).c_str());
	OutputDebugString(L"\n");
	*/

	int map_padding = 40;

	for (int i = 0; i < m_WorldState.explosive_count; ++i) {
		memcpy(&explosion_info, ptr, sizeof(TilePos));
		ptr += sizeof(TilePos);

		AddDynamicObject(BitmapKey::EXPLOSION, Vector2(explosion_info.pos) + map_padding);
	}

	for (int i = 0; i < m_WorldState.bomb_count; ++i) {
		memcpy(&bomb_info, ptr, sizeof(SendBombInfo));
		ptr += sizeof(SendBombInfo);

		AddDynamicObject(BitmapKey::BOMB, Vector2(bomb_info.pos) + map_padding);
	}

	for (int i = 0; i < m_WorldState.player_count; ++i) {
		memcpy(&player_info, ptr, sizeof(PlayerInfo));
		ptr += sizeof(PlayerInfo);

		switch (player_info.state) {
		case PlayerState::UP:
			degree[player_info.id] = -90.f;
			break;
		case PlayerState::DOWN:
			degree[player_info.id] = 90.f;
			break;
		case PlayerState::LEFT:
			degree[player_info.id] = 180.f;
			break;
		case PlayerState::RIGHT:
			degree[player_info.id] = 0.f;
			break;
		case PlayerState::DEAD:
			ranking[curr_rank++] = player_info.id;

			for(int i = 0; i < 4; ++i)
				OutputDebugStringA(std::to_string(ranking[i]).c_str());

			break;
		default:
			break;
		}

		float opacity;
		if (player_info.no_damage_count > 0) opacity = 0.5f;
		else opacity = 1.f;

		AddDynamicObject(
			GetBitmapKeyFrom(player_info.id), 
			Vector2(player_info.pos) + map_padding,
			degree[player_info.id],
			opacity
		);

		for (int life = 0; life < player_info.life_count; ++life) {
			Vector2 pos{ ui_pos[player_info.id].x 
				+ (60 * life * ((player_info.id % 2) ? -1 : 1)) ,
				ui_pos[player_info.id].y 
			};
			AddDynamicObject(BitmapKey::UI_HEART, pos);
		}
	}
	
	m_Lock.unlock();
}

void ObjectContainer::Reset()
{
	m_StaticObjects.clear();
	m_DynamicObjects.clear();
	for (auto& text : m_Texts) {
		delete text;
		text = nullptr;
	}
	m_Texts.clear();
}

TextObject* ObjectContainer::AddText()
{
	TextObject* text = new TextObject();
	m_Texts.push_back(text);
	return text;
}
