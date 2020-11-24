#pragma once
#include "Scene.h"
class LoginScene : public Scene
{
public:
	LoginScene(GameFramework* framework);
	virtual ~LoginScene();

	void Initialize() override;
	void Update() override;
	void Update(PlayerInfo& info) {};

	void HandleInput(UINT message, WPARAM wParam, LPARAM lParam) override;
};

