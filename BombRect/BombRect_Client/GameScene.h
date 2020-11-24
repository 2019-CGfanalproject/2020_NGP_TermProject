#pragma once
#include "Scene.h"

class Player;
class GameScene : public Scene
{
public:
	GameScene(GameFramework* framework);
	virtual ~GameScene();

	Player* m_Player;
public:
	void Initialize() override;
	void Update() override;
	void Update(PlayerInfo& info);
	void HandleInput(UINT message, WPARAM wParam, LPARAM lParam) override;
};

