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

void ObjectContainer::Reset()
{
	m_StaticObjects.clear();
	m_DynamicObjects.clear();
}
