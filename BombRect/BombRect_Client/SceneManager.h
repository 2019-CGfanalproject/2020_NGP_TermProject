#pragma once
#include "Scene.h"

class GameFramework;
class SceneManager
{
	GameFramework* m_Framework;

	std::array<Scene*, (int)SceneID::COUNT> m_Scenes;
	Scene* m_CurScene;

public:
	SceneManager();
	~SceneManager();

	void Initialize(GameFramework* framework);
	void UpdateCurrentScene();
	void HandleInput();

	// Scene을 전환을 위한 메소드 제공
	void StartScene(SceneID id);
	void StopScene();

	Scene* GetCurrScene() {
		return m_CurScene;
	}
};
