#include "pch.h"
#include "ResultScene.h"
#include "GameFramework.h"

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

	for (int i = 0; i < 4; ++i) {
		int n = m_Framework->m_Objects.ranking[i];
		OutputDebugString(std::to_wstring(i).c_str());
		OutputDebugString(L", ");
		OutputDebugString(std::to_wstring(n).c_str());
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
