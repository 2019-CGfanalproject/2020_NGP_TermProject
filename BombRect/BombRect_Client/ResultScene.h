#pragma once
#include "Scene.h"

class ResultScene : public Scene
{
public:
	ResultScene(GameFramework* framework);
	virtual ~ResultScene();
public:
	void Initialize() override;
	void Destroy() override;
	void Update() override;
	void HandleInput(InputHandler* inputHandler) { }
	void HandleInput(UINT message, WPARAM wParam, LPARAM lParam) { }
};
