#include "pch.h"
#include "SceneManager.h"
#include "GameFramework.h"
#include "GameScene.h"
#include "LoginScene.h"

SceneManager::SceneManager()
{
	m_Framework = nullptr;
	m_CurScene = nullptr;
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

void SceneManager::Initialize(GameFramework* framework)
{
	// Scene들을 생성한다.
	m_Framework = framework;
	m_Scenes[(int)SceneID::LOGIN] = new LoginScene{ framework };
	m_Scenes[(int)SceneID::GAME] = new GameScene{ framework };

	m_CurScene = m_Scenes[0];
	m_CurScene->Initialize();
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

void SceneManager::ChangeScene(SceneID id)
{
	// m_CurScene->Destroy();
	m_CurScene = m_Scenes[(int)id];
	m_CurScene->Initialize();
}
