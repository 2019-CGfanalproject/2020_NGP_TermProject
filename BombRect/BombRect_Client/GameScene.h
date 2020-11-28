#pragma once
#include "Scene.h"

class Player;
class GameScene : public Scene
{
public:
	PlayerInfo m_Players[4];
	SendBombInfo m_Bombs[12];

public:
	GameScene(GameFramework* framework);
	virtual ~GameScene();

	Player* m_Player;
public:
	void Initialize() override;
	void Destroy() override;

	void Update() override;
	void Update(PlayerInfo& info);
	void Update(PlayerInfo players[], SendBombInfo bombs[]);
	void HandleInput(UINT message, WPARAM wParam, LPARAM lParam) override;
};
