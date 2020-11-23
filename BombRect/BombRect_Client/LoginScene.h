#pragma once
#include "Scene.h"
class LoginScene : public Scene
{
public:
	LoginScene(GameFramework* framework);
	virtual ~LoginScene();

	void Initialize() override;
	void Update() override;
	void HandleInput() override;
};

