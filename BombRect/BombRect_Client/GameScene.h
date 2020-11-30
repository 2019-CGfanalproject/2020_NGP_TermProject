#pragma once
#include "Scene.h"

class GameScene : public Scene
{
	PlayerState m_PlayerState;

public:
	GameScene(GameFramework* framework);
	virtual ~GameScene();

public:
	void Initialize() override;
	void Destroy() override;

	void Update() override;
	void Update(PlayerInfo& info);
	void HandleInput(UINT message, WPARAM wParam, LPARAM lParam) override;
	void HandleInput(InputHandler* inputHandler) override;
};
