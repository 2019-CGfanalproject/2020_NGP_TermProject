#pragma once
#include "Scene.h"

class GameScene : public Scene
{
public:
	GameScene(GameFramework* framework);
	virtual ~GameScene();

public:
	void Initialize() override;
	void Update() override;
	void HandleInput() override;
};

