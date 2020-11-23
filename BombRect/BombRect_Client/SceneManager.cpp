#include "pch.h"
#include "SceneManager.h"
#include "GameFramework.h"
#include "GameScene.h"

SceneManager::SceneManager()
{
	m_Framework = nullptr;
	m_Scenes.fill(nullptr);
	m_CurScene = nullptr;
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

void SceneManager::Initialize(GameFramework* framework)
{
	m_Framework = framework;
	m_Scenes[0] = new GameScene;
	m_CurScene = m_Scenes[0];
}

void SceneManager::UpdateCurrentScene()
{
	if (m_CurScene)
		m_CurScene->Update();
}

void SceneManager::HandleInput()
{
	if (m_CurScene)
		m_CurScene->HandleInput();
}

void SceneManager::StartScene(SceneID id)
{
}

void SceneManager::StopScene()
{
}
