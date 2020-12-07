#include "pch.h"
#include "ResultScene.h"
#include "GameFramework.h"

Vector2 ranking_pos[3] = {
	{360, 483},
	{160, 533},
	{560, 583},
};

ResultScene::ResultScene(GameFramework* framework)
	: Scene(framework, SceneID::RESULT)
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
	m_Framework->m_Objects.AddStaticObject(BitmapKey::RESULT, Vector2(0, 0));
	m_Framework->m_Objects.AddStaticObject(BitmapKey::TEAM_ICON, Vector2(600, 700));

	int i = 0;
	for (int id : m_Framework->m_Objects.ranking) {
		m_Framework->m_Objects.AddStaticObject(GetBitmapKeyFrom(id), ranking_pos[i++]);
		OutputDebugString(std::to_wstring(id).c_str());
		OutputDebugString(L"\n");
	}
}

void ResultScene::Destroy()
{
}

void ResultScene::Update()
{
	m_Framework->m_Objects.m_DynamicObjects.clear();
}
