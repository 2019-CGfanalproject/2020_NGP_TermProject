#include "pch.h"
#include "ObjectContainer.h"
#include "GameObject.h"

void ObjectContainer::AddStaticObject(BitmapKey key, Vector2 pos)
{
	m_StaticObjects.emplace_back(key, pos);
}
