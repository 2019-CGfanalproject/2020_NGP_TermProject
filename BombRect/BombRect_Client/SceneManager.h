#pragma once
#include "Scene.h"
#include "../packets.h"

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
	void UpdateCurrentScene(PlayerInfo& info);
	void UpdateCurrentScene(game_packet::SC_WorldState world);
	void UpdateCurrentScene(PlayerInfo players[], SendBombInfo bombs[]);
	void UpdateCurrentScene();
	void HandleInput(UINT message, WPARAM wParam, LPARAM lParam);

	// Scene을 전환을 위한 메소드 제공
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
