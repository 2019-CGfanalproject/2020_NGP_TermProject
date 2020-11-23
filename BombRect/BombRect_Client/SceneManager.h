#pragma once
#include "Scene.h"

class GameFramework;
class SceneManager
{
	GameFramework* m_Framework;

	Scene* m_CurScene;
	std::array<Scene*, (int)SceneID::COUNT> m_Scenes;

public:
	SceneManager();
	~SceneManager();

	void Initialize(GameFramework* framework);
	void UpdateCurrentScene();
	void HandleInput();

	// Scene�� ��ȯ�� ���� �޼ҵ� ����
	void StartScene(SceneID id);
	void StopScene();
	void ChangeScene(SceneID id);

	Scene* GetCurrScene() {
		return m_CurScene;
	}
};
