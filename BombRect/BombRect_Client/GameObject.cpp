#include "pch.h"
#include "GameObject.h"

//void DynamicObject::Update(Vector2 pos)
//{
//	m_Pos = pos;
//}

Character::Character(int id, BitmapKey key, Vector2 pos)
	:DynamicObject(key, pos)
{
	m_UserID = id;
}

void Character::Update(Vector2 pos)
{
}
