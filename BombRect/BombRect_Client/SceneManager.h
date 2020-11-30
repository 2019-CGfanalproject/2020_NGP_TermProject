#pragma once
#include "Scene.h"
#include "InputHandler.h"

class GameFramework;
class InputHandler;

class SceneManager
{
	GameFramework* m_Framework;
	InputHandler m_InputHandler;

	Scene* m_CurScene;
	std::array<Scene*, (int)SceneID::COUNT> m_Scenes;

public:
	SceneManager();
	~SceneManager();

	void Initialize(GameFramework* framework);
	void UpdateCurrentScene();
	void HandleInput(UINT message, WPARAM wParam, LPARAM lParam);

	// Scene�� ��ȯ�� ���� �޼ҵ� ����
	void StartScene(SceneID id);
	void StopScene();
	void ChangeScene(SceneID id);

	Scene* GetCurrScene() {
		return m_CurScene;
	}

	SceneID GetCurrSceneID() {
		return m_CurScene->GetID();
	}
};
