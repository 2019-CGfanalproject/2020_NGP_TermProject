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
		if (i >= 3) break;
		if (i == 0) {
			TextObject* nickname = m_Framework->m_Objects.AddText();
			nickname->m_Text = m_Framework->m_Objects.m_Nicknames[id].name;
			nickname->m_Left = 225;
			nickname->m_Right = 225 + 350;
			nickname->m_Top = 275;
			nickname->m_Bottom = 400;
			nickname->font = FontKey::LOGIN;
			nickname->is_align_center = true;
		}

		m_Framework->m_Objects.AddStaticObject(GetBitmapKeyFrom(id), ranking_pos[i++]);
		OutputDebugString(std::to_wstring(id).c_str());
		OutputDebugString(L"\n");
	}
}

void ResultScene::Destroy()
{
	m_Framework->m_Objects.ranking.clear();
}

void ResultScene::Update()
{
	m_Framework->m_Objects.m_DynamicObjects.clear();
}
