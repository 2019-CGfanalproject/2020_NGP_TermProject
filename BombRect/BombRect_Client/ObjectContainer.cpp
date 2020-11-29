#include "pch.h"
#include "ObjectContainer.h"
#include "GameObject.h"

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
	lock.lock();
	for (int i = 0; i < 100; ++i) {
		if (explo_info[i].r == -1) break;
		AddDynamicObject(BitmapKey::EXPLOSION,
			Vector2{ (int)explo_info[i].r * 80 + 40, (int)explo_info[i].c * 80  + 40});
	}
	lock.unlock();
}

void ObjectContainer::Reset()
{
	m_StaticObjects.clear();
	m_DynamicObjects.clear();
}
