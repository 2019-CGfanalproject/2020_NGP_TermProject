#include "pch.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
	m_Scenes.fill(nullptr);
}

SceneManager::~SceneManager()
{
	for (Scene* scene : m_Scenes) {
		if (scene) {
			delete scene;
			scene = nullptr;
		}
	}
}

void SceneManager::UpdateCurrentScene()
{
	if (m_CurScene)
		m_CurScene->Update();
}

void SceneManager::RenderCurrentScene()
{
	if (m_CurScene)
		m_CurScene->Render();
}

void SceneManager::HandleInput()
{
	if (m_CurScene)
		m_CurScene->HandleInput();
}
