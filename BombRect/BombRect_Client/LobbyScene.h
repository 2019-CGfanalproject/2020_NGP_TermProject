#pragma once
#include "Scene.h"

class LobbyScene : public Scene
{
	// ready 버튼 오브젝트

public:
	LobbyScene(GameFramework* framework)
		:Scene(framework, SceneID::LOBBY) { }
	virtual ~LobbyScene() { }

	void Initialize();
	void Destroy() override;

	void Update();
	void Update(PlayerInfo& info);
	void HandleInput(UINT message, WPARAM wParam, LPARAM lParam);
};

